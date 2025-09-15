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

def load_n_prepare_dataset(ds_name):
    
    if ds_name == "sven":
        data_dir = '/home/wyu3/workspace/sven/data_eval/trained'
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

def make_step_rewards(logits, token_masks, criterion2, temperature=0.9):
    all_scores_res = []
    for sample, token_mask in zip(logits, token_masks):
        # sample: (seq_len, num_labels)
        probs = sample[token_mask].softmax(dim=-1)  # (num_steps, 2)
        process_reward = probs[:, 1] - probs[:, 0]  # (num_steps,)
        if criterion2 == 'softmax':
        # weighted sum to approx. min, highly recommend when BoN eval and Fine-tuning LLM
            weight = torch.softmax(
                -process_reward / temperature, 
                dim=-1,
            )
            process_reward = weight * process_reward
            all_scores_res.append(process_reward.cpu().tolist())
            
        elif criterion2 == 'last_position':
            process_reward = [process_reward[-1]]
            all_scores_res.append(process_reward)
        else:
            raise ValueError(f'Invalid criterion2: {criterion2}')
    return all_scores_res

def get_args():
    
    parser = argparse.ArgumentParser()
    parser.add_argument('--model_name', type=str, default='Qwen/Qwen2.5-7B-Instruct')
    parser.add_argument('--output_dir', type=str, default='/project/flame/wyu3/PRM/sven/sven_trained_100')
    parser.add_argument('--criterion2', type=str, choices=['softmax', 'last_position'], default='softmax')
    parser.add_argument('--criterion1', type=str, choices=['binary', 'min', 'ave', 'acc'], default='binary')
    parser.add_argument('--prm_name', type=str, default='Qwen/Qwen2.5-Math-PRM-7B')
    parser.add_argument('--generate_answers', type=bool, default=False)
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
    
    
    prm_name = '/project/flame/wyu3/PRM/output/stage2/rep4/checkpoint-4500'
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
    
    generate_answers = False
    
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
                
                
                steps = [i.rstrip() for i in generated_code.split(step_separator)]
                input_ids = question_ids.clone()

                score_ids = []
                for step in steps:
                    step_ids = prm_tokenizer(
                        step, 
                        add_special_tokens=False, 
                        return_tensors='pt',
                    )['input_ids']
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
                    step_reward = make_step_rewards(logits, token_masks, criterion2)  # step_reward < 0 means vulnerable, >= 0 means safe
                    if criterion1 == 'binary':
                        if any(i < 0 for i in step_reward[0]):
                            outcome_reward = 0
                        else:
                            outcome_reward = 1
                        outcome_scores.append(outcome_reward)
                    elif criterion1 == 'min':
                        outcome_reward = torch.tensor(step_reward).min()
                        outcome_scores.append(outcome_reward.item())
                        
                    elif criterion1 == 'ave':
                        outcome_reward = torch.tensor(step_reward).sum(dim=-1)
                        outcome_scores.append(outcome_reward.item())
                    elif criterion1 == 'acc':
                        pred_or = np.sum(step_reward)
                        print(f'pred_or: {pred_or}, label: {label}')
                        
                        if (label == 0 and pred_or <= 0) or (label == 1 and pred_or > 0):
                            outcome_reward = 1
                        else:
                            outcome_reward = 0
                        
                        outcome_scores.append(outcome_reward)
                    else:
                        raise ValueError(f'Invalid criterion1: {criterion1}')

            
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
            
            
            if len(outcome_scores) > 0:
                print('--------------------------------')
                ## acc
                if criterion1 == 'acc':
                    print(f'acc: {np.mean(outcome_scores)}')
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
    if criterion1 != 'acc':           
        for k in [1, 3, 5]:
            print(f'top_{k}_safe_ratios:', np.mean(top_k_safe_ratios[k]))
            print(f'best_{k}_possible_safe_ratios:', np.mean(best_possible_safe_ratios[k]))
            print('average_safe_ratios:', np.mean(average_safe_ratios[k]))
    else:
        print(f'acc: {np.mean(acc_scores)}')

if __name__ == "__main__":
    main()