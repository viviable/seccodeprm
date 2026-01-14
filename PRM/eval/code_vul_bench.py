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
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from accelerate import Accelerator
from datasets import load_dataset, load_from_disk
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
    

def plot_accuracy_histograms(acc_hist, dataset_name, output_prefix="acc_hist"):
    """Draw and save accuracy/count histograms grouped by token length."""
    if not acc_hist:
        print("No accuracy data to plot.")
        return

    token_lengths = np.array([t[0] for t in acc_hist])
    matches = np.array([t[1] for t in acc_hist], dtype=float)

    # 定义分箱（对数分箱处理长尾）
    bins = np.logspace(np.log10(max(1, token_lengths.min())), 
                       np.log10(token_lengths.max()), 25)
    
    # 计算每个 bin 的统计量
    bin_indices = np.digitize(token_lengths, bins)
    bin_counts = []
    bin_acc = []
    bin_centers = []
    
    for i in range(1, len(bins)):
        mask = bin_indices == i
        if mask.sum() > 0:
            bin_counts.append(mask.sum())
            bin_acc.append(matches[mask].mean())
            bin_centers.append((bins[i-1] + bins[i]) / 2)
    
    bin_counts = np.array(bin_counts)
    bin_acc = np.array(bin_acc)
    bin_centers = np.array(bin_centers)

    # === 图1: Accuracy by token length ===
    fig, ax = plt.subplots(figsize=(8, 4))
    bars = ax.bar(range(len(bin_centers)), bin_acc, color="#5a9", edgecolor='white', alpha=0.9)
    
    # x 轴标签显示 token length 范围
    tick_labels = [f"{int(bins[i])}-{int(bins[i+1])}" for i in range(len(bin_centers))]
    ax.set_xticks(range(len(bin_centers)))
    ax.set_xticklabels(tick_labels, rotation=45, ha='right', fontsize=8)
    
    ax.set_xlabel("Token length range")
    ax.set_ylabel("Accuracy")
    ax.set_title("Accuracy by token length")
    ax.set_ylim(0, 1)
    ax.grid(True, axis='y', linestyle='--', alpha=0.9)
    plt.tight_layout()
    plt.savefig(f"{output_prefix}_acc.png", dpi=150)
    plt.close(fig)

    # === 图2: Counts by token length ===
    fig, ax = plt.subplots(figsize=(8, 4))
    ax.bar(range(len(bin_centers)), bin_counts, color="#6c8cd5", edgecolor='white', alpha=0.9)
    
    ax.set_xticks(range(len(bin_centers)))
    ax.set_xticklabels(tick_labels, rotation=45, ha='right', fontsize=8)
    
    ax.set_xlabel("Token length range")
    ax.set_ylabel("Number of examples")
    ax.set_title("Example counts by token length")
    ax.grid(True, axis='y', linestyle='--', alpha=0.9)
    plt.tight_layout()
    plt.savefig(f"{output_prefix}_counts.png", dpi=150)
    plt.close(fig)

    # === 图3 (可选): 合并图，双 y 轴 ===
    
    fig, ax1 = plt.subplots(figsize=(10, 5))
    
    x = np.arange(len(bin_centers))
    width = 0.6
    
    # 左 y 轴: counts
    bars1 = ax1.bar(x, bin_counts, width, color="#acd6ec", alpha=0.9, label='Count')
    ax1.set_ylabel("Number of examples", color="black",fontsize=20)
    ax1.tick_params(axis='y', labelcolor="black",labelsize=16)
    
    # 右 y 轴: accuracy
    ax2 = ax1.twinx()
    ax2.plot(x, bin_acc, 'o-', color="#f5a889", linewidth=2, markersize=6, label='Accuracy')
    ax2.set_ylabel("Accuracy", color="black",fontsize=20)
    ax2.tick_params(axis='y', labelcolor="black",labelsize=16)
    ax2.set_ylim(0, 1)
    
    ax1.set_xticks(x)
    ax1.set_xticklabels(tick_labels, rotation=45, ha='right', fontsize=10)
    ax1.set_xlabel("Token length range", fontsize=20)
    ax1.set_title(f"{dataset_name} ", fontsize=20)
    
    fig.legend(loc='upper right', bbox_to_anchor=(0.9, 0.9), fontsize=20)
    plt.tight_layout()
    plt.savefig(f"{output_prefix}_combined.png", dpi=150)
    plt.close(fig)

def main(args):
    
    bs = args.batch_size
    num_of_workers = args.num_of_workers
    separator = args.separator
    model_path = args.model
    temperature = args.temperature
    dataset_name = args.dataset_name

    model_name = model_path.split('/')[-1]

    configs = {
        'primevul_test_paired': load_dataset('vivi-yu/primevul_processed_dataset')["test"],
        'primevul_test_unpaired':  load_dataset('vivi-yu/primevul_processed_dataset_unpaired')["test"],
        "precise": load_dataset('vivi-yu/vul_code_precise')["test"],
        "reposvul_test": load_dataset('vivi-yu/reposvul_processed_dataset')["test"],
        "sven": load_dataset('vivi-yu/vul_code_sven')["val"],
    
    }

    accelerator = Accelerator()
    if accelerator.is_main_process:
        print(f'args: {args}')
    print(f'Loading model from {model_path}')
    model = transformers.AutoModelForTokenClassification.from_pretrained(model_path)
    model = model.bfloat16()
    tokenizer = transformers.AutoTokenizer.from_pretrained(model_path)
    model = accelerator.prepare(model)
    model.eval()
    
    dataset = configs[dataset_name]
    number_of_vulnerable = sum([0 in dataset[i]['labels'] for i in range(len(dataset))])
    print(f'total number of vulnerable {number_of_vulnerable}, total number: {len(dataset)}')
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
        if input_ids.size(-1) > 70000:
            print('input_ids.size(-1)', input_ids.size(-1))
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
            new_batch[i]['input_ids_length'] = input_ids[i].size(-1)
            if criterion == 'softmax':
                pred_or = ((-score / temperature).softmax(dim=-1) * score).sum()
                if (label_ != -1 and pred_or <= 0) or (label_ == -1 and pred_or > 0):
                    new_batch[i]['match'] = True
                else:
                    new_batch[i]['match'] = False
                if pred_or <= 0:
                    new_batch[i]['pred'] = 1
                else:
                    new_batch[i]['pred'] = 0
                    
            elif criterion == 'simple':
                if (label_ != -1 and score[-1] <= 0) or (label_ == -1 and score[-1] > 0):
                    new_batch[i]['match'] = True
                else:
                    new_batch[i]['match'] = False
                if score[-1] <= 0:
                    new_batch[i]['pred'] = 1
                else:
                    new_batch[i]['pred'] = 0
            elif criterion == 'allsteps':
                acc_allsteps_per_sample = [(labels[i][j]==1) == (score.cpu()>0)[j] for j in range(len(score))]
                new_batch[i]['match'] = np.array(acc_allsteps_per_sample).mean()
                new_batch[i]['predicted_label'] = (score.cpu()<=0).tolist()
                # print('acc_allsteps_per_sample', acc_allsteps_per_sample)
                print('new_batch[i]["match"]', new_batch[i]['match'])
            elif criterion == 'precise':
                first_zero = find_first_zero(score)
                if first_zero == label_:
                    new_batch[i]['match'] = True
                else:
                    new_batch[i]['match'] = False
            else:
                raise ValueError(f'Invalid criterion: {criterion}')
        res_data.extend(new_batch)
    print('length of res_data', len(res_data))
    clear_cache()
    
    accelerator.wait_for_everyone()
    gathered_data = gather_objects(res_data, accelerator)
    
    
    
    if accelerator.is_main_process:
        acc_hist = []
        print('length of gathered_data', len(gathered_data))
        for e in gathered_data:
            acc_hist.append((e['input_ids_length'], e['match']))
        plot_accuracy_histograms(
            acc_hist,
            dataset_name,
            output_prefix=f"{dataset_name}_{criterion}"
        )

    if accelerator.is_main_process and criterion != 'allsteps':
        
        data1 = [e for e in gathered_data if 0 in e['labels']]  # vulnerable
        print('length of vulnerable', len(data1))
        data2 = [e for e in gathered_data if 0 not in e['labels']]  # non-vulnerable
        print('length of non-vulnerable', len(data2))
        
        acc1 = np.mean([e['match'] for e in data1]) * 100
        acc2 = np.mean([e['match'] for e in data2]) * 100
        c1 = 2 * acc1 * acc2 / (acc1 + acc2)
        ave_acc = np.mean([e['match'] for e in gathered_data]) * 100
        
        print(f'{dataset_name} error acc: {acc1}, correct acc: {acc2}, c1: {c1}, ave_acc: {ave_acc}')

        TP = np.sum([e['match'] for e in data1])
        FP = np.sum([not e['match'] for e in data2])
        FN = np.sum([not e['match'] for e in data1])
        TN = np.sum([e['match'] for e in data2])
        
        precision = TP / (TP + FP)
        recall = TP / (TP + FN) 
        f1 = 2 * precision * recall / (precision + recall)
        print('TP', TP, 'FP', FP, 'FN', FN, 'TN', TN)
        print('FPR', FP/(FP+TN), 'FNR', FN/(FN+TP))
        print(f'{dataset_name} precision: {precision}, recall: {recall}, f1: {f1}')
        
        ### calculate pairwise
        if dataset_name == 'primevul_test_paired':
            PC = 0
            PV = 0
            PB = 0
            PR = 0
            for i in range(len(gathered_data)):
                index = gathered_data[i]['index']
                for j in range(len(gathered_data)):
                    if gathered_data[j]['index'] == index and gathered_data[j]['labels'] != gathered_data[i]['labels']:
                        paired_data = gathered_data[j]
                        
                        if paired_data['match'] and gathered_data[i]['match']:
                            PC += 1
                        elif paired_data['pred'] and gathered_data[i]['pred']:
                            PV += 1
                        elif not paired_data['pred'] and not gathered_data[i]['pred']:
                            PB += 1
                        elif not paired_data['match'] and not gathered_data[i]['match']:
                            PR += 1
                        
            if accelerator.is_main_process  and criterion != 'allsteps':
                sum_=PC+PV+PB+PR
                print(f'{dataset_name} PC: {PC/sum_}, PV: {PV/sum_}, PB: {PB/sum_}, PR: {PR/sum_}')
    if accelerator.is_main_process and criterion == 'allsteps':
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
        
        print(f'{dataset_name} precision: {precision}, recall: {recall}, f1: {f1}')
        print(f"Accuracy: {(TP + TN) / (TP + TN + FP + FN)}")
        print(f"Total number of results: {len(predicted_labels)}")
        
    if accelerator.distributed_type == "MULTI_GPU":
        import torch.distributed as dist
        dist.destroy_process_group()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-m", "--model", type=str)
    parser.add_argument("-b", "--batch_size", type=int, default=1)
    parser.add_argument("-w", "--num_of_workers", type=int, default=4)
    parser.add_argument("-s", "--separator", type=str, default="\n\n", help="It's important to use the same separator as the one used during TRL training")
    parser.add_argument("-t", "--temperature", type=float, default=0.1)
    parser.add_argument("-c", "--criterion", choices=["softmax", "simple", "allsteps", "precise"], type=str, default="softmax")
    parser.add_argument("-d", "--dataset_name", choices=["sven","primevul_test_paired", "primevul_test_unpaired", "precise","reposvul_test", "reposvul_test_concat", "reposvul_test_func"], type=str, default="primevul_test_paired")
    args = parser.parse_args()

    set_seed(42)
    os.environ["TOKENIZERS_PARALLELISM"] = "false"
    print('args', args)
    main(args)
