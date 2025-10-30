# torchrun --nproc_per_node=4 process_bench.py --model <model_path> -b 24 -w 4 -s "\n"
import argparse
import json
import os
import random
from copy import deepcopy
import gc
import numpy as np
import torch
import transformers
from accelerate import Accelerator
from datasets import load_dataset, load_from_disk, Dataset
from torch.utils.data import DataLoader, DistributedSampler
from tqdm import tqdm


def collate_fn(batch, tokenizer, separator = '\n\n'):
    input_ids = []
    score_ids = []
    label = []
    separator_ids = tokenizer.encode(separator, add_special_tokens=False, return_tensors='pt')
    for i in batch:
        prompt_ids = tokenizer(i['prompt'], add_special_tokens=False, return_tensors='pt')['input_ids']
        score_ids.append([])
        for completion in i['completions']:
            completion_ids = tokenizer(completion, add_special_tokens=False, return_tensors='pt')['input_ids']
            prompt_ids = torch.cat([prompt_ids, completion_ids, separator_ids], dim=-1)
            score_ids[-1].append(prompt_ids.size(-1) - 1)
        if all(_ == 1 for _ in i['labels']):
            label.append(-1)
        else:
            label.append(i['labels'].index(0))
        input_ids.append(prompt_ids)
    
    # right pad input_ids
    pad_token_id = tokenizer.pad_token_id
    max_len = max([i.size(-1) for i in input_ids])
    for i, input_idx in enumerate(input_ids):
        input_ids[i] = torch.cat([
            input_idx.squeeze(), 
            torch.LongTensor(
                [pad_token_id] * (max_len - input_idx.size(-1))
            )
        ])
    input_ids = torch.stack(input_ids)
    labels = [i['labels'] for i in batch]
    
    return dict(
        input_ids=input_ids,
        label=label,
        labels=labels,
        score_ids=score_ids
    )
    
    
def find_first_zero(tensor):
    zeros = (tensor == 0).nonzero()
    return zeros[0].item() if zeros.numel() > 0 else -1

def gather_objects(data, accelerator):
    world_size = accelerator.num_processes
    if world_size == 1:
        return data
        
    all_data = [None] * world_size
    torch.distributed.all_gather_object(all_data, data)
    
    if accelerator.is_main_process:
        result = []
        for process_data in all_data:
            result.extend(process_data)
        return result
    return None

def set_seed(seed):
    random.seed(seed)
    np.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed_all(seed)

def clear_cache():
    torch.cuda.empty_cache()
    torch.cuda.ipc_collect()
    gc.collect()

def load_data(dataset_name):
    if dataset_name == 'sven_train':
        dataset = load_from_disk("/project/flame/wyu3/PRM/sven_processed_dataset")['train']
    elif dataset_name == 'sven_train_no_py':
        dataset = load_from_disk("/project/flame/wyu3/PRM/sven_processed_dataset_no_py")['train']
    elif dataset_name == 'sven_val':
        dataset = load_from_disk("/project/flame/wyu3/PRM/sven_processed_dataset")['test']
    elif dataset_name == 'sven_val_no_py':
        dataset = load_from_disk("/project/flame/wyu3/PRM/sven_processed_dataset_no_py")['val']
    elif dataset_name == 'bigvul_test':
        dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset_one_zero_dedup_test")
    elif dataset_name == 'precisebugs_test':
        dataset = load_dataset('vivi-yu/vul_code_precise')['test']
        # dataset = dataset.select(range(100))
    elif dataset_name == 'primevul_test':
        dataset = load_from_disk("/project/flame/wyu3/PRM/primevul_processed_dataset")['test']
        dataset = dataset.select(range(100))
    elif dataset_name == 'reposvul_test':
        dataset = load_from_disk("/project/flame/wyu3/PRM/reposvul_processed_dataset")['test']
        dataset = dataset.select(range(100))
    else:
        raise ValueError(f'Invalid dataset name: {dataset_name}')
    return dataset

def gather_results_from_one_model(dataloader, tokenizer, separator, model, accelerator, temperature, criterion='softmax'):
    res_data = []
    for batch_ in tqdm(dataloader, disable=not accelerator.is_main_process):
        new_batch = deepcopy(batch_)

        batch = collate_fn(batch_, tokenizer, separator)
        input_ids = batch['input_ids'].to(accelerator.device)
        input_ids = input_ids.to(torch.long)
        score_ids = batch['score_ids']
        with accelerator.autocast(), torch.no_grad():
            outputs = model(input_ids)
            logits = outputs.logits
        

        for i, score_id in enumerate(score_ids):
            prob = logits[i, score_id].softmax(dim=-1)  # (steps, 2)
            score = prob[:, 1] - prob[:, 0]  # (steps,)
            
            pred_or = ((-score / temperature).softmax(dim=-1) * score).sum()
                
            new_batch[i]['confidence'] = pred_or
            new_batch[i]['prob'] = prob.cpu()
            new_batch[i]['score'] = score.detach().to(torch.float32).cpu().numpy()  # tensor to numpy array
            new_batch[i]['label'] = (0 in new_batch[i]['labels'])
                
        res_data.extend(new_batch)
    print('length of res_data', len(res_data))
    clear_cache()
    
    accelerator.wait_for_everyone()
    gathered_data = gather_objects(res_data, accelerator)
    return gathered_data

def calculate_metrics(results, accelerator, dataset_name, criterion='softmax'):
    if accelerator.is_main_process:
        
        moe_results = []
        len_results = len(results[0])
        expert_num = len(results)
        if criterion == 'softmax':
            threshold = 0.5
            for i in range(len_results):
                weighted_results = [e[i]['confidence']*int(e[i]['confidence']>0) for e in results]
                predicted_label = int(sum(weighted_results)/expert_num <= threshold)
                print('predicted_label', predicted_label, 'gt_label', int(results[0][i]['label']))
                moe_results.append({'predicted_label': predicted_label, 'gt_label': int(results[0][i]['label'])})
            
        elif criterion == 'last_position':
            threshold = 0.5
            for i in range(len_results):
                weighted_results = [e[i]['score']*(e[i]['score']>0) for e in results]
                predicted_label = int(sum(weighted_results)/expert_num <= threshold)
                print('predicted_label', predicted_label, 'gt_label', int(results[0][i]['label']))
                moe_results.append({'predicted_label': predicted_label, 'gt_label': int(results[0][i]['label'])})
        elif criterion == 'allsteps':
            threshold = 0.5
            for i in range(len_results):
                array_score = []
                for e in results:
                    array_score += [e[i]['score']>0]*e[i]['score']
                    
                array_score.sum(axis=1)
                predicted_label = (array_score/expert_num <= threshold)
                gt_label = results[0][i]['labels']
                print('predicted_label', predicted_label, 'gt_label', gt_label)
                moe_results.append({'predicted_label': predicted_label, 'gt_label': gt_label!=-1})
            
    if criterion != 'allsteps':
        
        data1 = [e for e in moe_results if e['gt_label']]  # vulnerable
        data2 = [e for e in moe_results if not e['gt_label']]  # safe
        
        vul_acc = np.sum([e['predicted_label'] == e['gt_label'] for e in data1]) / len(data1)
        safe_acc = np.sum([e['predicted_label'] == e['gt_label'] for e in data2]) / len(data2)
        print(f'{dataset_name} vul_acc: {vul_acc}, safe_acc: {safe_acc}')
        
        TP = np.sum([e['predicted_label'] == 1 and e['gt_label'] == 1 for e in moe_results])
        FP = np.sum([e['predicted_label'] == 1 and e['gt_label'] == 0 for e in moe_results])
        FN = np.sum([e['predicted_label'] == 0 and e['gt_label'] == 1 for e in moe_results])
        TN = np.sum([e['predicted_label'] == 0 and e['gt_label'] == 0 for e in moe_results])
        print('TP', TP, 'FP', FP, 'FN', FN, 'TN', TN)
        precision = TP / (TP + FP) if TP + FP > 0 else 0
        recall = TP / (TP + FN) if TP + FN > 0 else 0
        f1 = 2 * precision * recall / (precision + recall) if precision + recall > 0 else 0
        accuracy = (TP + TN) / (TP + TN + FP + FN) if TP + TN + FP + FN > 0 else 0
        print(f'{dataset_name} precision: {precision}, recall: {recall}, f1: {f1}')
        
    elif criterion == 'allsteps':
        predicted_labels = [e['predicted_label'] for e in moe_results]
        gt_labels = [e['gt_label'] for e in moe_results]
        predicted_labels = np.concatenate(predicted_labels)
        gt_labels = np.concatenate(gt_labels)
        accuracy = np.sum(predicted_labels == gt_labels) / len(predicted_labels)
        print(f'{dataset_name} accuracy: {accuracy}')

def main(args):
    print('args', args)
    bs = args.batch_size
    num_of_workers = args.num_of_workers
    separator = args.separator
    temperature = args.temperature
    dataset_name = args.dataset_name
    
    model_paths = {
        "sven_prm": "/project/flame/wyu3/PRM/output/stage2/sven_s2_last_10epoch",
        "repos_prm": "/project/flame/wyu3/PRM/output/stage2/reposvul_s2_2epoch_s2_526",
        "precise_prm": "/project/flame/wyu3/PRM/output/stage2/precisebugs_s2_1epoch_s1_918",
        "primevul_prm": "/project/flame/wyu3/PRM/output/stage2/primevul_s2_3epoch_s1_s2",
    }

    accelerator = Accelerator()
    dataset = load_data(dataset_name)
    # dataset = dataset.select(range(3))
    print('total number of vulnerable', sum([0 in dataset[i]['labels'] for i in range(len(dataset))]))
    results = []
    for model_name, model_path in model_paths.items():
        print(f'Loading model from {model_path}')
        model = transformers.AutoModelForTokenClassification.from_pretrained(model_path)
        tokenizer = transformers.AutoTokenizer.from_pretrained(model_path)
        model = model.bfloat16()
        model = accelerator.prepare(model)
        model.eval()

        
        
        sampler = None
        if accelerator.distributed_type == "MULTI_GPU":
            sampler = DistributedSampler(
                dataset,
                num_replicas=accelerator.num_processes,
                rank=accelerator.process_index,
                shuffle=False,
            )
        dataloader = DataLoader(
            dataset, 
            batch_size=bs, 
            collate_fn=lambda x: x, 
            num_workers=num_of_workers,
            sampler=sampler,
            drop_last=False,
        )

        gathered_data = gather_results_from_one_model(dataloader, tokenizer, separator, model, accelerator, temperature, args.criterion)
        results.append(gathered_data)
        
        del model
        del tokenizer
        clear_cache()
        
    calculate_metrics(results, accelerator, dataset_name, criterion=args.criterion)


    if accelerator.distributed_type == "MULTI_GPU":
        import torch.distributed as dist
        dist.destroy_process_group()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--dataset_name", choices=["sven_train", "sven_train_no_py", "sven_val", "sven_val_no_py", "bigvul_test", "precisebugs_test", "primevul_test", "reposvul_test"], type=str, default="sven_train")
    parser.add_argument("-b", "--batch_size", type=int, default=1)
    parser.add_argument("-w", "--num_of_workers", type=int, default=4)
    parser.add_argument("-s", "--separator", type=str, default="\n\n", help="It's important to use the same separator as the one used during TRL training")
    parser.add_argument("-t", "--temperature", type=float, default=0.1)
    parser.add_argument("-c", "--criterion", choices=["softmax", "last_position", "allsteps"], type=str, default="softmax")
    args = parser.parse_args()

    set_seed(42)
    os.environ["TOKENIZERS_PARALLELISM"] = "false"
    main(args)
