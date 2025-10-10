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
    gc.collect()

def load_data(dataset_name):
    dataset = load_from_disk("/project/flame/wyu3/PRM/reposvul_processed_dataset")['test']
    
    return dataset

def main(args):
    print('args', args)
    bs = args.batch_size
    num_of_workers = args.num_of_workers
    separator = args.separator
    model_path = args.model
    temperature = args.temperature
    dataset_name = args.dataset_name

    model_name = model_path.split('/')[-1]

    configs = {
        dataset_name: 1420,
    }

    accelerator = Accelerator()
    print(f'Loading model from {model_path}')
    model = transformers.AutoModelForTokenClassification.from_pretrained(model_path)
    tokenizer = transformers.AutoTokenizer.from_pretrained(model_path)
    model = accelerator.prepare(model)
    model.eval()

    for config, num in configs.items():
        # dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset")["test"]
        dataset = load_data(dataset_name)
        print('total number of vulnerable', sum([0 in dataset[i]['labels'] for i in range(len(dataset))]))
        # val_dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset_one_zero")["test"]
        # train_dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset_one_zero")["train"]
        # dataset = dataset.select(range(500))
        
        
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

        res_data = []
        for batch_ in tqdm(dataloader, disable=not accelerator.is_main_process):
            new_batch = deepcopy(batch_)

            batch = collate_fn(batch_, tokenizer, separator)
            input_ids = batch['input_ids'].to(accelerator.device)
            input_ids = input_ids.to(torch.long)
            if input_ids.size(-1) > 30000:
                print('input_ids.size(-1) > 30000', input_ids.size(-1))
                continue
            label = batch['label']
            labels = batch['labels']
            score_ids = batch['score_ids']
            with accelerator.autocast(), torch.no_grad():
                outputs = model(input_ids)
                logits = outputs.logits
            
            criterion = args.criterion
            for i, score_id in enumerate(score_ids):
                label_ = label[i]
                prob = logits[i, score_id].softmax(dim=-1)  # (steps, 2)
                score = prob[:, 1] - prob[:, 0]  # (steps,)
                if criterion == 'softmax':
                    pred_or = ((-score / temperature).softmax(dim=-1) * score).sum()
                    if (label_ != -1 and pred_or <= 0) or (label_ == -1 and pred_or > 0):
                        new_batch[i]['match'] = True
                    else:
                        new_batch[i]['match'] = False
                    print(pred_or)
                elif criterion == 'last_position':
                    if (label_ != -1 and score[-1] <= 0) or (label_ == -1 and score[-1] > 0):
                        new_batch[i]['match'] = True
                    else:
                        new_batch[i]['match'] = False
                elif criterion == 'allsteps':
                    
                    acc_allsteps_per_sample = [(labels[i][j]==1) == (score.cpu()>0)[j] for j in range(len(score))]
                    new_batch[i]['match'] = np.array(acc_allsteps_per_sample).mean()
                    new_batch[i]['predicted_label'] = (score.cpu()<=0).tolist()
                    # print('acc_allsteps_per_sample', acc_allsteps_per_sample)
                    print('new_batch[i]["match"]', new_batch[i]['match'])
                else:
                    raise ValueError(f'Invalid criterion: {criterion}')
            res_data.extend(new_batch)
        print('length of res_data', len(res_data))
        clear_cache()
        
        accelerator.wait_for_everyone()
        gathered_data = gather_objects(res_data, accelerator)

        if accelerator.is_main_process and criterion != 'allsteps':
            data1 = [e for e in gathered_data if 0 in e['labels']]  # vulnerable
            data2 = [e for e in gathered_data if 0 not in e['labels']]  # non-vulnerable
            
            acc1 = np.mean([e['match'] for e in data1]) * 100
            acc2 = np.mean([e['match'] for e in data2]) * 100
            c1 = 2 * acc1 * acc2 / (acc1 + acc2)
            print(f'{config} error acc: {acc1}, correct acc: {acc2}, c1: {c1}')

            TP = np.sum([e['match'] for e in data1])
            FP = np.sum([not e['match'] for e in data2])
            FN = np.sum([not e['match'] for e in data1])
            TN = np.sum([e['match'] for e in data2])
            print('TP', TP, 'FP', FP, 'FN', FN, 'TN', TN)
            
            precision = TP / (TP + FP)
            recall = TP / (TP + FN) 
            f1 = 2 * precision * recall / (precision + recall)
            print(f'{config} precision: {precision}, recall: {recall}, f1: {f1}')
            
        elif accelerator.is_main_process and criterion == 'allsteps':
            predicted_labels = [e['predicted_label'] for e in gathered_data]
            predicted_labels =  np.array(np.concatenate(predicted_labels))
            gt_labels = [e['labels'] for e in gathered_data]
            gt_labels = np.array(np.concatenate(gt_labels))
            print('predicted_labels', type(predicted_labels), 'gt_labels', type(gt_labels))
            
            
            TP = np.sum((predicted_labels == 1) & (gt_labels == 0))  #predicted_labels < 0 is vul, gt_labels = 0 is vulnerable
            FP = np.sum((predicted_labels == 1) & (gt_labels == 1))
            FN = np.sum((predicted_labels == 0) & (gt_labels == 0))
            TN = np.sum((predicted_labels == 0) & (gt_labels == 1))
            print('TP', TP, 'FP', FP, 'FN', FN, 'TN', TN)
            
            precision = TP / (TP + FP)
            recall = TP / (TP + FN)
            f1 = 2 * precision * recall / (precision + recall)
            print(f'{config} precision: {precision}, recall: {recall}, f1: {f1}')
            print(f"Accuracy: {(TP + TN) / (TP + TN + FP + FN)}")
            print(f"Total number of results: {len(predicted_labels)}")



    if accelerator.distributed_type == "MULTI_GPU":
        import torch.distributed as dist
        dist.destroy_process_group()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-m", "--model", type=str)
    parser.add_argument("-d", "--dataset_name", choices=["reposvul_test"], type=str, default="reposvul_test")
    parser.add_argument("-b", "--batch_size", type=int, default=1)
    parser.add_argument("-w", "--num_of_workers", type=int, default=4)
    parser.add_argument("-s", "--separator", type=str, default="\n\n", help="It's important to use the same separator as the one used during TRL training")
    parser.add_argument("-t", "--temperature", type=float, default=0.1)
    parser.add_argument("-c", "--criterion", choices=["softmax", "last_position", "allsteps"], type=str, default="softmax")
    args = parser.parse_args()

    set_seed(42)
    os.environ["TOKENIZERS_PARALLELISM"] = "false"
    main(args)
