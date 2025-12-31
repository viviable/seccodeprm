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
        # generated_dir  /project/flame/wyu3/PRM/bon/CWEval/evals/eval_251001_225018/generated_0/lang/c
        dist_data = {}
        generated_name = generated_dir.split('/')[-2] + '/' + generated_dir.split('/')[-1]
        ## load code
        for i_th_generated in os.listdir(generated_dir):
            if i_th_generated.endswith('.json'):
                continue
            ## i_th_generated: generated_12
            for task in os.listdir(os.path.join(generated_dir, i_th_generated)):
                if task not in ['core', 'lang']:
                    continue
                ## task: core or lang
                
                if task not in dist_data:
                    dist_data[task] = {}
                for language in os.listdir(os.path.join(generated_dir, i_th_generated, task)):
                    ## language: c or cpp or go or py or js
                    if language not in dist_data[task]:
                        dist_data[task][language] = {}
                    for file_name in os.listdir(os.path.join(generated_dir, i_th_generated, task, language)):
                        ## file_name: cwe_020_0_c_test.py
                        if 'cache' in file_name or 'compile' in file_name:
                            continue
                        with open(os.path.join(generated_dir, i_th_generated, task, language, file_name)) as f:
                            code = f.read()
                        cwe_id = file_name.split('_')[1]
                        if cwe_id not in dist_data[task][language]:
                            dist_data[task][language][cwe_id] = {}
                    
                        new_data = {
                            'code': code, 
                            'task': task,
                            'language': language,
                            'index': i_th_generated,
                            'cwe_id': cwe_id, # 020
                            'file_path': os.path.join(generated_name, i_th_generated, task, language),
                        }
                        dist_data[task][language][cwe_id][i_th_generated] = new_data
            ### load labels
            with open(os.path.join(generated_dir, i_th_generated, 'res.json'), 'r') as f:
                res = json.load(f)
                for file_path, labels in res.items():
                    # file_path: evals/eval_251001_225018/generated_13/core/c/cwe_020_0_c_test.py
                    task = file_path.split('/')[-3]
                    language = file_path.split('/')[-2]
                    file_name = file_path.split('/')[-1]
                    cwe_id = file_name.split('_')[1]
                    dist_data[task][language][cwe_id][i_th_generated]['label_functional'] = labels['functional']
                    dist_data[task][language][cwe_id][i_th_generated]['label_secure'] = labels['secure']
                    
                    
        return dist_data

def bon(candidates_dict, criterion2, criterion1, temperature=0.1):
    # criterion2 is for how to aggregate the step rewards
    bon_scores = {}
    for gen_index, single_data in candidates_dict.items():
        step_reward = single_data['step_reward']
        if criterion2 == 'softmax':
        # weighted sum to approx. min, highly recommend when BoN eval and Fine-tuning LLM
            weight = torch.softmax(
                -step_reward / temperature, 
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
    parser.add_argument('--output_dir', type=str, default='/home/wyu3/workspace/PURE/PRM/eval/sven_trained')
    parser.add_argument('--criterion2', type=str, choices=['softmax', 'plain', 'advantage'], default='softmax')
    parser.add_argument('--criterion1', type=str, choices=['binary', 'min', 'ave', 'last_position'], default='ave')
    parser.add_argument('--prm_name', type=str, default='/project/flame/wyu3/PRM/output/stage2/coder_s2_all_31340_froms1_2epoch/checkpoint-7435')
    parser.add_argument('--generate_answers', type=bool, default=False)
    parser.add_argument('--generated_dir', type=str, default='/project/flame/wyu3/PRM/bon/CWEval/evals/eval_251001_225018')
    return parser.parse_args()


def calculate_metrics(bon_scores, top_k=5):
    sorted_candidates = sorted(bon_scores.items(), key=lambda x: x[1]['bon_score'], reverse=True)
    num_candidates = len(sorted_candidates)
    
    k_values = [1, 3, 5, num_candidates]
    correct = {
        'functional': {k: 0 for k in k_values},
        'secure': {k: 0 for k in k_values}
    }

    for k in k_values:
        top_k = sorted_candidates[:k]
        
        # Top-k is correct if ANY of top-k has correct label
        if any(c['label_functional'] for c in top_k):
            correct['functional'][k] += 1
        
        if any(c['label_secure'] for c in top_k):
            correct['secure'][k] += 1
    
    # Calculate accuracy
    accuracy = {
        'functional': {k: correct['functional'][k] / num_candidates for k in k_values},
        'secure': {k: correct['secure'][k] / num_candidates for k in k_values}
    }
    return accuracy

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
    return step_rewards  # (num_steps,)
    

def main():
    args = get_args()
    
    testset = load_n_prepare_dataset("cweval", args.generated_dir)
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
    prm_tokenizer = AutoTokenizer.from_pretrained(
        prm_name, 
        trust_remote_code=True,
    )

    
    top_k_safe_ratios = {}
    best_possible_safe_ratios = {}
    average_safe_ratios = {}
    acc_scores = []
    for k in [1, 3, 5]:
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
    for single_data in testset:
        # sampled_ids = np.random.choice(range(len(ds_item)), size=100, replace=False)
        
        generated_code = single_data['code']
        # generated_code = generated_code.replace('#include <stdio.h>', '')
        # generated_code = generated_code.replace('#include <stdlib.h>', '')
        # generated_code = generated_code.replace('#include <limits.h>', '')
        # generated_code = generated_code.replace('#include <string.h>', '')
        # generated_code = generated_code.replace('#include <stdbool.h>', '')
                
        step_reward = get_stepwise_rewards(generated_code, step_separator, step_separator_token, question_ids, prm, prm_tokenizer)
        
        single_data['step_reward'] = step_reward
    
    ## calculate top_k_safe_ratio, best_possible_safe_ratio, average_safe_ratio
    for task in testset:
        for language in testset[task]:
            for cwe_id in testset[task][language]:
                subset = testset[task][language][cwe_id]
                bon_scores = bon(subset, criterion2, criterion1)
                metrics = calculate_metrics(bon_scores)
                print(metrics)
                subset['metrics'] = metrics

if __name__ == "__main__":
    main()