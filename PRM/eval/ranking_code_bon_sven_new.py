import numpy as np
import torch
from datasets import load_from_disk
from tqdm import tqdm
from transformers import AutoTokenizer, AutoModelForTokenClassification, AutoModel
import os
import json
from vllm import LLM, SamplingParams
from constant import CWES_TRAINED
from sven_eval import LMCodeEvaler
import csv
import argparse
import re

def load_n_prepare_dataset(ds_name):
    
    if ds_name == "sven":
        data_dir = '/project/flame/wyu3/PRM/sven_git/sven/data_eval/trained'  # /project/flame/wyu3/PRM/bon/CWEval/evals/eval_qwen3_30b/generated_0/lang/c
        testset = {}
        vul_types = CWES_TRAINED
        
        for vul_type in vul_types:
            testset[vul_type] = []
            vul_dir = os.path.join(data_dir, vul_type)
            
            new_data = {}
            for scenario in list(sorted(os.listdir(vul_dir))):
                new_data[scenario] = {}
                with open(os.path.join(vul_dir, scenario, 'info.json')) as f:
                    info = json.load(f)
                new_data[scenario]['info'] = info
                with open(os.path.join(vul_dir, scenario, 'file_context.'+info['language'])) as f:
                    file_context = f.read()
                with open(os.path.join(vul_dir, scenario, 'func_context.'+info['language'])) as f:
                    func_context = f.read()
                new_data[scenario]['file_context'] = file_context
                new_data[scenario]['func_context'] = func_context
            testset[vul_type].append(new_data)
        return testset
    
def bon(candidates_dict, criterion2, criterion1, temperature=0.1):
    # criterion2 is for how to aggregate the step rewards
    bon_scores = {}
    for gen_index, single_data in candidates_dict.items():
        step_reward = torch.tensor(single_data['step_reward'])
        if criterion2 == 'softmax':
        # weighted sum to approx. min, highly recommend when BoN eval and Fine-tuning LLM
            weight = torch.softmax(
                -1 * step_reward / temperature, 
                dim=-1,
            )
            process_reward = weight * step_reward

        elif criterion2 == 'plain':
            process_reward = step_reward
        elif criterion2 == 'advantage':
            process_reward = step_reward - step_reward.min(-1).values
        else:
            raise ValueError(f'Invalid criterion2: {criterion2}')
        
        if criterion1 == 'last_position':
            final_score = process_reward[-1].cpu()
        elif criterion1 == 'min':
            final_score = process_reward.min(-1).values.cpu()
        elif criterion1 == 'ave':
            final_score = process_reward.mean(-1).cpu()
        elif criterion1 == 'binary':
            final_score = (step_reward>0).sum(-1).cpu()
        else:
            raise ValueError(f'Invalid criterion1: {criterion1}')
        bon_scores[gen_index] = {'bon_score': final_score, 'label_functional': single_data['label_functional'], 'label_secure': single_data['label_secure']}
    return bon_scores

def calculate_metrics(bon_scores, top_k=5):
    sorted_candidates = sorted(bon_scores.items(), key=lambda x: x[1]['bon_score'], reverse=True)
    num_candidates = len(sorted_candidates)

    
    k_values = [1, 3, 5, 10, num_candidates]
    correct = {
        'functional': {k: 0 for k in k_values},
        'secure': {k: 0 for k in k_values}
    }

    for k in k_values:
        top_k = sorted_candidates[:k]
        # Top-k is correct if ANY of top-k has correct label
        if any( c[1]['label_functional'] for c in top_k ):
            correct['functional'][k] += 1
        
        if any([c[1]['label_secure'] for c in top_k]):
            correct['secure'][k] += 1
    
   
    return correct


def _metric_sort_key(metric_key):
    if metric_key == "acc":
        return (0, 0, 0, metric_key)
    match = re.match(r'^(top|best)_(\d+)_', metric_key)
    if match:
        group_order = {"top": 0, "best": 1}
        return (1, int(match.group(2)), group_order.get(match.group(1), 2), metric_key)
    if metric_key.startswith("average"):
        return (2, 0, 0, metric_key)
    return (3, 0, 0, metric_key)


def write_metrics_csv(csv_path, run_name, average_metrics):
    metric_keys = sorted(average_metrics.keys(), key=_metric_sort_key)
    with open(csv_path, 'a', newline='') as f:
        writer = csv.writer(f)
        header = ['name']
        header.extend(metric_keys)
        writer.writerow(header)

        row = [run_name]
        for key in metric_keys:
            row.append(average_metrics[key])
        writer.writerow(row)
        
def get_stepwise_rewards(generated_code, step_separator, step_separator_token, question_ids, prm, prm_tokenizer):
    steps = [i.rstrip() for i in generated_code.split(step_separator)]
    input_ids = question_ids.clone()

    score_ids = []
    for step in steps:
        step_ids = prm_tokenizer(step, add_special_tokens=False, return_tensors='pt')['input_ids']
        input_ids = torch.cat(
            [input_ids, step_ids, step_separator_token],
            dim=-1,
        )
        score_ids.append(input_ids.size(-1) - 1)

    input_ids = input_ids.to(prm.device, dtype=torch.long)
    token_masks = torch.zeros_like(input_ids, dtype=torch.bool)
    token_masks[0, score_ids] = True
    assert torch.all(input_ids[token_masks].to("cpu") == step_separator_token)

    with torch.no_grad():
        logits = prm(input_ids).logits
        step_rewards = []
        for sample, token_mask in zip(logits, token_masks):
            # sample: (seq_len, num_labels)
            probs = sample[token_mask].softmax(dim=-1)  # (num_steps, 2)
            process_reward = probs[:, 1] - probs[:, 0]  # (num_steps,)
            step_rewards.append(process_reward)
    # Expect single-sample batch; return tensor directly for downstream math.
    if len(step_rewards) != 1:
        raise ValueError(f"Expected single-sample rewards, got {len(step_rewards)}")
    return step_rewards[0]  # (num_steps,)

def bon_score_from_step_reward(step_reward, criterion2, criterion1, temperature=0.1):
    if criterion2 == 'softmax':
        weight = torch.softmax(
            -1 * step_reward / temperature,
            dim=-1,
        )
        process_reward = weight * step_reward
    elif criterion2 == 'plain':
        process_reward = step_reward
    elif criterion2 == 'advantage':
        process_reward = step_reward - step_reward.min(-1).values
    else:
        raise ValueError(f'Invalid criterion2: {criterion2}')

    if criterion1 == 'last_position':
        final_score = process_reward[-1].cpu()
    elif criterion1 == 'min':
        final_score = process_reward.min(-1).values.cpu()
    elif criterion1 == 'ave':
        final_score = process_reward.mean(-1).cpu()
    elif criterion1 == 'binary':
        final_score = (step_reward > 0).sum(-1).cpu()
    else:
        raise ValueError(f'Invalid criterion1: {criterion1}')
    return final_score.item()

def str2bool(value):
    if isinstance(value, bool):
        return value
    value = value.lower()
    if value in ("true", "1", "yes", "y"):
        return True
    if value in ("false", "0", "no", "n"):
        return False
    raise argparse.ArgumentTypeError(f"Invalid boolean value: {value}")

def get_args():
    
    parser = argparse.ArgumentParser()
    parser.add_argument('--model_name', type=str, default='Qwen/Qwen2.5-7B-Instruct')
    parser.add_argument('--output_dir', type=str, default='/project/flame/wyu3/PRM/sven_trained/')
    parser.add_argument('--criterion2', type=str, choices=['softmax', 'plain', 'advantage'], default='softmax')
    parser.add_argument('--criterion1', type=str, choices=['binary', 'min', 'ave', 'last_position', 'acc'], default='binary')
    parser.add_argument('--prm_name', type=str, default='vivi-yu/precise_s1')
    parser.add_argument('--generate_answers', type=str2bool, nargs='?', const=True, default=False)
    parser.add_argument('--name', type=str, default='test')
    
    return parser.parse_args()

def main():
    testset = load_n_prepare_dataset("sven")
    args = get_args()
    model_name = args.model_name
    device = "auto"
    # output_dir = "/project/flame/wyu3/PRM/sven/sven_trained_100"
    output_dir = args.output_dir
    criterion2 = args.criterion2
    criterion1 = args.criterion1
    prm_name = args.prm_name
    

    if 'qwen' in prm_name.lower():
        prm = AutoModel.from_pretrained(
            prm_name, 
            device_map=device, 
            dtype=torch.bfloat16,
            trust_remote_code=True,
        ).eval()
    else:
        prm = AutoModelForTokenClassification.from_pretrained(
            prm_name, 
            device_map=device, 
            torch_dtype=torch.bfloat16,
            trust_remote_code=True,
        ).eval()
    num_gpus = torch.cuda.device_count()
    prm_tokenizer = AutoTokenizer.from_pretrained(
        prm_name, 
        trust_remote_code=True,
    )
    
    generate_answers = args.generate_answers
    
    if generate_answers:
        llm = LLM(model_name, 
                tensor_parallel_size=num_gpus, 
                gpu_memory_utilization=0.90, 
                max_model_len=8192,
                trust_remote_code=True)
        tokenizer = AutoTokenizer.from_pretrained(
            model_name, 
            trust_remote_code=True,
        )
        sampling_params = SamplingParams(
            temperature=0.8,
            top_p=0.95,
            max_tokens=512,
            n=100
        )
        
    step_separator = "\n\n"
    step_separator_token = prm_tokenizer(
        step_separator, 
        add_special_tokens=False, 
        return_tensors='pt',
    )['input_ids']

    if generate_answers:
        evaler = LMCodeEvaler(sampling_params, llm)
        results = evaler.generate_testset_answers(testset, output_dir)
        
    top_k_safe_ratios = {}
    best_possible_safe_ratios = {}
    average_safe_ratios = {}
    acc_scores = []
    for k in [1, 3, 5]:
        top_k_safe_ratios[k] = []
        best_possible_safe_ratios[k] = []
        average_safe_ratios[k] = []
    
    for vul_type, data_scenario in testset.items():
        # sampled_ids = np.random.choice(range(len(ds_item)), size=100, replace=False)
        safe_count = 0
        total_count = 0
        
            
        assert len(data_scenario) == 1
        scenario_single_data = data_scenario[0]
        for scenario, single_data in scenario_single_data.items():
            out_src_dir = os.path.join(output_dir, vul_type, scenario, 'output')
            
            question = 'Determine whether the code is vulnerable or not.'
            question_ids = prm_tokenizer(
                question, 
                add_special_tokens=False, 
                return_tensors='pt',
            )['input_ids']
            outcome_scores = []
            labels = []
            True_negative = 0
            True_positive = 0
            False_negative = 0
            False_positive = 0
            for fname in os.listdir(out_src_dir):
                if fname.endswith('.o') or fname.endswith('.py') or 'Makefile' in fname:
                    continue
                csv_path = os.path.join(output_dir, vul_type, scenario, 'codeql.csv')
                label = 1
                with open(csv_path, 'r') as f:
                    reader = csv.reader(f)
                    for row in reader:
                        # print('/'+fname, row)
                        if  '/'+fname in row:
                            label = 0
                            break
                labels.append(label)
                
                with open(os.path.join(out_src_dir, fname), 'r') as f:
                    generated_code = f.read()
                
                generated_code = generated_code.replace('#include <stdio.h>', '')
                generated_code = generated_code.replace('#include <stdlib.h>', '')
                generated_code = generated_code.replace('#include <limits.h>', '')
                generated_code = generated_code.replace('#include <string.h>', '')
                generated_code = generated_code.replace('#include <stdbool.h>', '')

                step_reward = get_stepwise_rewards(
                    generated_code,
                    step_separator,
                    step_separator_token,
                    question_ids,
                    prm,
                    prm_tokenizer,
                )
                if criterion1 == 'acc':
                    pred_or = step_reward.sum().item()
                    print(f'step_reward: {step_reward.tolist()}')
                    print(f'pred_or: {pred_or}, label: {label}')

                    if (label == 0 and pred_or <= 0):
                        True_negative += 1
                    elif (label == 1 and pred_or > 0):
                        True_positive += 1
                    elif (label == 0 and pred_or > 0):
                        False_negative += 1
                    elif (label == 1 and pred_or <= 0):
                        False_positive += 1
                    else:
                        raise ValueError(f'Invalid label: {label}')

                    outcome_reward = 2 * True_positive / (2 * True_positive + False_positive + False_negative)
                    outcome_scores.append(outcome_reward)
                else:
                    outcome_reward = bon_score_from_step_reward(step_reward, criterion2, criterion1)
                    outcome_scores.append(outcome_reward)

            
            ## for eval
            def eval_top_k(k):
                # top_k_scores = np.sort(outcome_scores)[-k:]
                top_k_labels = [labels[i] for i in np.argsort(outcome_scores)[-k:]]
                
                top_k_safe_ratio = np.mean(top_k_labels)
                
                best_labels = np.sort(labels)[-k:]
                best_possible_safe_ratio = np.mean(best_labels)
                
                average_safe_ratio = np.mean(labels)
                return  top_k_safe_ratio, best_possible_safe_ratio, average_safe_ratio
            
            def eval_best():
                best_idx = np.argmax(outcome_scores)
                if labels[best_idx] == 1:
                    safe_count += 1
                total_count += 1
                return safe_count, total_count
            
            average_metrics = {}
            if len(outcome_scores) > 0:
                print('--------------------------------')
                ## acc
                if criterion1 == 'acc':
                    print(f'True_positive: {True_positive}, False_positive: {False_positive}, True_negative: {True_negative}, False_negative: {False_negative}')
                    
                    print(f'acc: {True_positive+True_negative/(True_positive + False_positive+True_negative+False_negative)}')
                    if True_positive + False_positive != 0:
                        print(f'Precision: {True_positive/(True_positive + False_positive)}')
                    if True_positive + False_negative != 0:
                        print(f'Recall: {True_positive/(True_positive + False_negative)}')
                    if 2*True_positive + False_positive + False_negative != 0:
                        print(f'F1: {2*True_positive/(2*True_positive + False_positive + False_negative)}')
                    else:
                        print(f'F1: 0')
                    acc_scores.extend(outcome_scores)
                else:
                    for k in [1, 3, 5]:
                        top_k_safe_ratio, best_possible_safe_ratio, average_safe_ratio = eval_top_k(k)
                        top_k_safe_ratios[k].append(top_k_safe_ratio)
                        best_possible_safe_ratios[k].append(best_possible_safe_ratio)
                        average_safe_ratios[k].append(average_safe_ratio)
                        print(f'top_{k}_safe_ratio: {top_k_safe_ratio}, best_possible_safe_ratio: {best_possible_safe_ratio}, average_safe_ratio: {average_safe_ratio}')
                # best_idx = np.argmax(outcome_scores)
                # safe_count, total_count = eval_best()
                # print(f'best_safe_ratio: {safe_count/total_count}')
                # print(f'{vul_type}{scenario}:',labels[best_idx], outcome_scores[best_idx])
                # # print('outcome_scores:',np.sort(outcome_scores))
                # # print('labels:',[labels[i] for i in np.argsort(outcome_scores)])
                # print('outcome_scores:',outcome_scores)
                # print('labels:',labels)
                with open(os.path.join(output_dir, vul_type, scenario, 'result.jsonl'), 'r') as f:
                    for line in f:
                        print(f'{vul_type}{scenario}:',line)
                        
    print('--------------------------------')     
    average_metrics = {}
    if criterion1 != 'acc':
        for k in [1, 3, 5]:
            average_metrics[f'top_{k}_safe_ratios'] = np.mean(top_k_safe_ratios[k])
            average_metrics[f'best_{k}_possible_safe_ratios'] = np.mean(best_possible_safe_ratios[k])
            average_metrics[f'average_safe_ratios'] = np.mean(average_safe_ratios[k])
    else:
        average_metrics['acc'] = np.mean(acc_scores)
    write_metrics_csv("sven_exp.csv", args.name, average_metrics)
    
if __name__ == "__main__":
    main()
