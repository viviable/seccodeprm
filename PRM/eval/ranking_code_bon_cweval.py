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

def load_n_prepare_dataset(ds_name, generated_dir):
    
    if ds_name == "cweval":
        # generated_dir  /project/flame/wyu3/PRM/bon/CWEval/evals/eval_251001_225018
        dist_data = {}
        generated_name = os.path.basename(os.path.dirname(generated_dir)) + '/' + os.path.basename(generated_dir)
        evals_root = generated_dir

        # Build data strictly from res.json to keep alignment with evaluation.
        for gen_dir in os.listdir(generated_dir):
            if gen_dir.endswith('.json'):
                continue
            res_json_path = os.path.join(generated_dir, gen_dir, 'res.json')
            if not os.path.exists(res_json_path):
                continue
            with open(res_json_path, 'r') as f:
                res = json.load(f)
            for file_path, labels in res.items():
                # file_path: evals/eval_251001_225018/generated_13/core/c/cwe_020_0_c_test.py
                file_path = file_path.replace('test', 'task')
                
                norm_path = os.path.normpath(file_path)
                parts = norm_path.split(os.sep)
                file_name = parts[-1]
                language = parts[-2]
                task = parts[-3]
                gen_id = parts[-4] if len(parts) >= 4 else gen_dir
                task_id = os.path.splitext(file_name)[0]
                file_path = file_path.replace('.py', f'.{language}')

                if task not in dist_data:
                    dist_data[task] = {}
                if language not in dist_data[task]:
                    dist_data[task][language] = {}
                if task_id not in dist_data[task][language]:
                    dist_data[task][language][task_id] = {}

                if os.path.isabs(file_path) and os.path.exists(file_path):
                    fs_path = file_path
                else:
                    candidates = []
                    candidates.append(
                        os.path.join(evals_root, '/'.join(file_path.split('/')[2:]))
                    )
                    candidates.append(
                        os.path.join(evals_root, '/'.join(file_path.split('/')[3:]))
                    )
                    candidates.append(
                        os.path.join(evals_root, '/'.join(file_path.split('/')[4:]))
                    )
                   
                    # import pdb; pdb.set_trace()    
                    fs_path = next(
                        (p for p in candidates if os.path.exists(p)),
                        None,
                    )
                    if fs_path is None:
                        raise FileNotFoundError(
                            f"Could not resolve path from res.json: {file_path}"
                        )
                with open(fs_path) as f:
                    code = f.read()

                new_data = {
                    'code': code,
                    'task': task,
                    'language': language,
                    'index': gen_id,
                    'task_id': task_id,  # cwe_020_0_c_test
                    'file_path': os.path.join(generated_name, gen_id, task, language),
                    'label_functional': labels['functional'],
                    'label_secure': labels['secure'],
                }
                dist_data[task][language][task_id][gen_id] = new_data
        return dist_data

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

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--criterion2', type=str, choices=['softmax', 'plain', 'advantage'], default='softmax')
    parser.add_argument('--criterion1', type=str, choices=['binary', 'min', 'ave', 'last_position'], default='ave')
    parser.add_argument('--prm_name', type=str, default='vivi-yu/sven_s2_f1_71')
    parser.add_argument('--generated_dir', type=str, default='/project/flame/wyu3/PRM/bon/CWEval/evals/eval_251001_225018')
    parser.add_argument('--name', type=str, default='run')
    return parser.parse_args()

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


def write_metrics_csv(csv_path, run_name, average_metrics, num_groups):
    k_values = sorted(average_metrics['functional'].keys())
    with open(csv_path, 'a', newline='') as f:
        writer = csv.writer(f)
        header = ['name', 'num_groups']
        for k in k_values:
            header.append(f'functional@{k}')
            header.append(f'secure@{k}')
        writer.writerow(header)

        row = [run_name, num_groups]
        for k in k_values:
            row.append(average_metrics['functional'][k])
            row.append(average_metrics['secure'][k])
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
    

def main():
    args = get_args()
    
    testset = load_n_prepare_dataset("cweval", args.generated_dir)
    
    
    device = "auto"
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
    prm_tokenizer = AutoTokenizer.from_pretrained(
        prm_name, 
        trust_remote_code=True,
    )

    
    top_k_safe_ratios = {}
    best_possible_safe_ratios = {}
    average_safe_ratios = {}
    for k in [1, 3, 5, 10]:
        top_k_safe_ratios[k] = []
        best_possible_safe_ratios[k] = []
        average_safe_ratios[k] = []
    
    step_separator = "\n\n"
    step_separator_token = prm_tokenizer(
        step_separator, 
        add_special_tokens=False, 
        return_tensors='pt',
    )['input_ids']
    question = 'Determine whether the code is vulnerable or not.'
    question_ids = prm_tokenizer(
        question, 
        add_special_tokens=False, 
        return_tensors='pt',
    )['input_ids']
    for task in testset:
        for language in testset[task]:
            for cwe_id in testset[task][language]:
                subset = testset[task][language][cwe_id]
                # sampled_ids = np.random.choice(range(len(ds_item)), size=100, replace=False)
                for i_th_generated in subset.keys():
                    generated_code = subset[i_th_generated]['code']
                    # generated_code = generated_code.replace('#include <stdio.h>', '')
                    # generated_code = generated_code.replace('#include <stdlib.h>', '')
                    # generated_code = generated_code.replace('#include <limits.h>', '')
                    # generated_code = generated_code.replace('#include <string.h>', '')
                    # generated_code = generated_code.replace('#include <stdbool.h>', '')
                            
                    step_reward = get_stepwise_rewards(generated_code, step_separator, step_separator_token, question_ids, prm, prm_tokenizer)
                    
                    subset[i_th_generated]['step_reward'] = step_reward
    
    ## calculate top_k_safe_ratio, best_possible_safe_ratio, average_safe_ratio
    average_metrics = {
        'functional': {},
        'secure': {},
    }
    num_groups = 0
    for task in testset:
        for language in testset[task]:
            for cwe_id in testset[task][language]:
                subset = testset[task][language][cwe_id]
                bon_scores = bon(subset, criterion2=criterion2, criterion1=criterion1)
                
                metrics = calculate_metrics(bon_scores)
                for k, v in metrics['functional'].items():
                    average_metrics['functional'][k] = average_metrics['functional'].get(k, 0) + v
                for k, v in metrics['secure'].items():
                    average_metrics['secure'][k] = average_metrics['secure'].get(k, 0) + v
                subset['metrics'] = metrics
                num_groups += 1
                
    for k in list(average_metrics['functional'].keys()):
        average_metrics['functional'][k] /= num_groups
    for k in list(average_metrics['secure'].keys()):
        average_metrics['secure'][k] /= num_groups
    print("average_metrics:", average_metrics)
    print("num_groups:", num_groups)
    write_metrics_csv("cweval_exp.csv", args.name, average_metrics, num_groups)


if __name__ == "__main__":
    main()
