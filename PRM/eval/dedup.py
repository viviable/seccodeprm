import concurrent.futures
from tqdm import tqdm
import math
import os
from datasets import load_from_disk, load_dataset, Dataset

def check_chunk(args):
    """处理数据块的函数"""
    chunk, train_completions = args
    
    repeat_count = 0
    new_chunk = []
    
    for index in tqdm(range(len(chunk))):
        # item is a dictionary from the dataset
        completion = chunk[index]['completions']
        if completion in train_completions:
            repeat_count += 1
        else:
            new_chunk.append(chunk[index])
    
    return repeat_count, new_chunk

def check_if_repeat_parallel(train_dataset, dataset, num_workers=None):
    if num_workers is None:
        num_workers = min(128, os.cpu_count())
    
    # 将train_dataset['completions']转换为set，加速查找
    train_completions = train_dataset['completions']
    
    # 将dataset分割成chunks
    chunk_size = math.ceil(len(dataset) / num_workers)
    chunks = [dataset.select(range(i, min(i + chunk_size, len(dataset)))) for i in range(0, len(dataset), chunk_size)]
    
    # 准备参数
    args_list = [(chunk, train_completions) for chunk in chunks]
    
    repeat_count = 0
    new_dataset = []
    
    # 使用ProcessPoolExecutor并行处理
    with concurrent.futures.ProcessPoolExecutor(max_workers=num_workers) as executor:
        # 提交任务并显示进度条
        futures = [executor.submit(check_chunk, args) for args in args_list]
        
        # 收集结果
        for future in tqdm(concurrent.futures.as_completed(futures), 
                          total=len(futures), desc="Processing chunks"):
            chunk_repeat, chunk_new = future.result()
            repeat_count += chunk_repeat
            new_dataset.extend(chunk_new)
    
    print('repeat_count', repeat_count)
    print('new_dataset', len(new_dataset))
    return new_dataset


def dedup_testset(test_dataset):
    seen_completions = set()
    new_test_dataset = []
    for item in tqdm(test_dataset):
        comp = ''.join(item['completions'])
        if comp in seen_completions:
            continue
        else:
            seen_completions.add(comp)
            new_test_dataset.append(item)
    return new_test_dataset


if __name__ == "__main__":
    
    ## dedup testset from trainset
    # test_dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset")["test"]
    # val_dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset")["validation"]
    # hf_data = load_dataset('bstee615/bigvul')
    # hf_data_test = hf_data["test"]
    # train_dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset")["train"]
    # new_dataset = check_if_repeat_parallel(train_dataset, test_dataset)
    
    ## dedup testset from testset
    test_dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset_dedup_test")
    new_dataset = dedup_testset(test_dataset)
    
    #save to disk
    new_dataset_ = Dataset.from_list(new_dataset)
    new_dataset_.save_to_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset_dedup_test_dedup")
    
    # print(sum([1 if 0 in item['labels'] else 0 for item in new_dataset]))