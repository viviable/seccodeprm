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
    if dataset_name == 'sven_train':
        dataset = load_from_disk("/project/flame/wyu3/PRM/sven_processed_dataset")['train']
    elif dataset_name == 'sven_train_no_py':
        dataset = load_from_disk("/project/flame/wyu3/PRM/sven_processed_dataset_no_py")['train']
    elif dataset_name == 'sven_val':
        dataset = load_from_disk("/project/flame/wyu3/PRM/sven_processed_dataset")['val']
    elif dataset_name == 'sven_val_no_py':
        dataset = load_from_disk("/project/flame/wyu3/PRM/sven_processed_dataset_no_py")['val']
    elif dataset_name == 'bigvul_test':
        dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset_one_zero_dedup_test")
    else:
        raise ValueError(f'Invalid dataset name: {dataset_name}')
    return dataset

def main(args):
    bs = args.batch_size
    num_of_workers = args.num_of_workers
    separator = args.separator
    model_path = args.model
    temperature = args.temperature
    dataset_name = args.dataset_name

    model_name = model_path.split('/')[-1]

    configs = {
        # 'bigvul_test': 34076, 
        # 'sven_train': 1420,
        'sven_val': 1420,
    }
    all_f1_scores = []

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
            label = batch['label']
            labels = batch['labels']
            score_ids = batch['score_ids']
            with accelerator.autocast(), torch.no_grad():
                outputs = model(input_ids)
                logits = outputs.logits
            
            # criterion = 'softmax'
            # criterion = 'simple'
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
                    # print('acc_allsteps_per_sample', acc_allsteps_per_sample)
                    print('new_batch[i]["match"]', new_batch[i]['match'])
                else:
                    raise ValueError(f'Invalid criterion: {criterion}')
            res_data.extend(new_batch)
        print('length of res_data', len(res_data))
        clear_cache()
        
        accelerator.wait_for_everyone()
        gathered_data = gather_objects(res_data, accelerator)

        if accelerator.is_main_process:
            data1 = [e for e in gathered_data if 0 in e['labels']]  # vulnerable
            data2 = [e for e in gathered_data if 0 not in e['labels']]  # non-vulnerable
            
            acc1 = np.mean([e['match'] for e in data1]) * 100
            acc2 = np.mean([e['match'] for e in data2]) * 100
            f1 = 2 * acc1 * acc2 / (acc1 + acc2)
            print(f'{config} error acc: {acc1:.1f}, correct acc: {acc2:.1f}, f1: {f1:.1f}')

            all_f1_scores.append(f1)

    if accelerator.is_main_process:
        print(f'ProcessBench. Average F1: {np.mean(all_f1_scores):.1f}')

    if accelerator.distributed_type == "MULTI_GPU":
        import torch.distributed as dist
        dist.destroy_process_group()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-m", "--model", type=str)
    parser.add_argument("-d", "--dataset_name", choices=["sven_train", "sven_train_no_py", "sven_val", "sven_val_no_py", "bigvul_test"], type=str, default="sven_train")
    parser.add_argument("-b", "--batch_size", type=int, default=1)
    parser.add_argument("-w", "--num_of_workers", type=int, default=4)
    parser.add_argument("-s", "--separator", type=str, default="\n", help="It's important to use the same separator as the one used during TRL training")
    parser.add_argument("-t", "--temperature", type=float, default=0.1)
    parser.add_argument("-c", "--criterion", choices=["softmax", "last_position", "allsteps"], type=str, default="softmax")
    args = parser.parse_args()

    set_seed(42)
    os.environ["TOKENIZERS_PARALLELISM"] = "false"
    main(args)
