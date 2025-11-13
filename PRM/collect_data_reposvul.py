from datasets import load_dataset, DatasetDict, Dataset, load_from_disk
from tqdm import tqdm
import json

from difflib import SequenceMatcher
from typing import List, Tuple

def compare_code_blocks(code1: str, code2: str) -> Tuple[List[dict], List[dict]]:
    """
    Compare two code blocks and return segmented parts with labels.
    
    Args:
        code1: First code block
        code2: Second code block
        
    Returns:
        Tuple of (list1, list2) where each list contains dicts with:
        - 'content': the code segment
        - 'type': 'same' or 'diff'
        - 'lines': line numbers in original code
    """
    
    def normalize_line(line: str) -> str:
        """Remove leading/trailing whitespace for comparison"""
        return line.strip()
    
    # Split into lines
    lines1 = code1.strip().split('\n')
    lines2 = code2.strip().split('\n')
    
    # Use SequenceMatcher to find matching blocks
    matcher = SequenceMatcher(None, lines1, lines2)
    matching_blocks = matcher.get_matching_blocks()
    
    list1 = []  # Segments for code1
    list2 = []  # Segments for code2
    
    i1, i2 = 0, 0  # Current line positions
    
    for match in matching_blocks:
        a, b, size = match.a, match.b, match.size
        
        # Handle different parts before this match
        if i1 < a:
            # Different segment in code1
            diff_lines1 = lines1[i1:a]
            list1.append({
                'content': '\n'.join(diff_lines1),
                'type': 'diff',
                'lines': f"{i1+1}-{a}" if i1+1 != a else str(i1+1)
            })
        
        if i2 < b:
            # Different segment in code2  
            diff_lines2 = lines2[i2:b]
            list2.append({
                'content': '\n'.join(diff_lines2),
                'type': 'diff',
                'lines': f"{i2+1}-{b}" if i2+1 != b else str(i2+1)
            })
        
        # If only one side has differences, add empty diff to other side
        if (i1 < a) != (i2 < b):
            if i1 >= a:  # code1 has no diff here, but code2 does
                list1.append({
                    'content': '',
                    'type': 'diff',
                    'lines': f"after-{i1}"
                })
            if i2 >= b:  # code2 has no diff here, but code1 does
                list2.append({
                    'content': '',
                    'type': 'diff', 
                    'lines': f"after-{i2}"
                })
        
        # Handle matching part
        if size > 0:
            same_lines1 = lines1[a:a+size]
            same_lines2 = lines2[b:b+size]
            
            list1.append({
                'content': '\n'.join(same_lines1),
                'type': 'same',
                'lines': f"{a+1}-{a+size}" if size > 1 else str(a+1)
            })
            
            list2.append({
                'content': '\n'.join(same_lines2),
                'type': 'same',
                'lines': f"{b+1}-{b+size}" if size > 1 else str(b+1)
            })
        
        i1, i2 = a + size, b + size
    
    return list1, list2

def print_reposvul_data():
    path = "/project/flame/wyu3/PRM/ReposVul/ReposVul_c.jsonl"

    with open(path, "r", encoding="utf-8") as f:
        has_function = 0
        for i, line in enumerate(f, start=1):
            line = line.strip()
            if not line:
                continue
            try:
                record = json.loads(line)
            except json.JSONDecodeError as e:
                # 解析错误时可记录或跳过
                print(f"Line {i}: JSON decode error: {e}")
                continue
            import pdb; pdb.set_trace()
            
            if 'function_before' in record['details'][0]:
                has_function += 1
                has_vul = sum(1 for i, item in enumerate(record['details'][0]['function_before']) if item['target']!=0 )
                if not has_vul:
                    print(record['details'][0]['function_before'] == record['details'][0]['function_after'])
                    
            # 如果只想做少量示例展示，可在达到 N 条后 break
            # if i >= 100: break
            # sum(1 for i, item in enumerate(record['details'][0]['function_before']) if item['target']==0 and item != record['details'][0]['function_after'][i])
            # sum(1 for i, item in enumerate(record['details'][0]['function_before']) if item['target']!= record['details'][0]['function_after'][i]['target'])
            # sum(1 for i, item in enumerate(record['details'][0]['function_before']) if item['target']!=0 )
            # sum(1 for i, item in enumerate(record['details'][0]['function_after']) if item['target']!=0 )
        
        print(has_function)

def postprocess_code_blocks(list1, list2):
    for i, block in enumerate(list2):
        if '/*' in block['content'] and '*/' not in block['content']:
            # find the next block that contains '*/'
            for j in range(i+1, len(list2)):
                if '*/' in list2[j]['content']:
                    list2[i]['content'] += ''.join([block['content'] for block in list2[i+1:j+1]])
                    list1[i]['content'] += ''.join([block['content'] for block in list1[i+1:j+1]])
                    
                    for k in range(i+1, j+1):
                        list2.pop(k)
                        list1.pop(k)
                break
    return list1, list2

def one_zero_dataset(path):
    dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset")
    new_dataset = DatasetDict()
    
    # Initialize empty lists for each split
    for split_name in ['test', 'train', 'validation']:
        new_dataset[split_name] = []
        
        for data in dataset[split_name]:
            new_data = {}
            if 0 in data['labels']:
                first_zero_index = data['labels'].index(0)
                
                new_data['labels'] = data['labels'].copy()  # Make a copy first
                # Set all labels from first_zero_index onwards to 0
                for i in range(first_zero_index, len(new_data['labels'])):
                    new_data['labels'][i] = 0
                new_data['completions'] = data['completions']
                new_data['prompt'] = data['prompt']
                new_data['source'] = data['source']
                new_data['other_info'] = data['other_info']
                new_data['index'] = data['index']
                new_dataset[split_name].append(new_data)
            else:
                new_dataset[split_name].append(data)
        new_dataset[split_name] = Dataset.from_list(new_dataset[split_name])
    new_dataset.save_to_disk(path)

def preprocess_data(data):
    for item in data:
        for key, value in item.items():
            if value == [] or (isinstance(value, list) and len(value) == 0):
                print(f"Empty list found in column: {key}")
                item[key] = None
            if key == 'detail_info' and isinstance(value, dict):
                for k, v in value.items():
                    # Convert ALL empty lists to None
                    if v == [] or (isinstance(v, list) and len(v) == 0):
                        print(f"Empty list found in column: {k}")
                        item['detail_info'][k] = None
                    # Keep non-empty values as-is
                    # Remove the redundant else branches
                        
    return data

def load_data(language_name):
    json_path = f'/project/flame/wyu3/PRM/ReposVul/ReposVul_{language_name}.jsonl' 
    data = []
    
    with open(json_path, 'r') as f:
        for line in f:
            single_data = json.loads(line)
            single_data['detail_info'] = {}
            for i, detail in enumerate(single_data['details']):
                if 'target' in detail:
                    single_data['detail_info'][f'{i}'] = detail   
            if single_data['detail_info'] == {}: ## remove the details without target
                continue
            single_data.pop('details')  ## change detials to detail_info
            data.append(single_data)
   
    dataset = preprocess_data(data)
    
    # Convert detail_info dict to JSON string for consistent schema
    for item in dataset:
        if 'detail_info' in item:
            item['detail_info'] = json.dumps(item['detail_info'])
    
    dataset = Dataset.from_list(dataset)
    return dataset

def main(path):
    ## 
    # Create DatasetDict once to hold all splits
    vul_dataset = DatasetDict()
    
    # Process each split
    language_name = ['c', 'cpp', 'java', 'python' ] #
    train_data = []
    test_data = [] 
    for split_name in language_name:
        print(f"Processing {split_name} split...")
        
        split_data = []
        
        dataset = load_data(split_name)
        for index, data in enumerate(tqdm(dataset, desc=f"Processing {split_name}")):
            detail_info= json.loads(data['detail_info'])
            for i, i_detail in detail_info.items():
                target = i_detail.get('target')
                if target == 0 or target == -1:
                    code = i_detail['code']
                    code_before = i_detail['code_before']
                    completions = code.split('\n\n')
                    prompt = 'Determine whether the code is vulnerable or not.'
                    labels = len(completions)*[1]
                    source = 'REPOSVUL'
                    # other_info = { k: data[k] for k in data.keys() }
                    
                    split_data.append({
                        'prompt': prompt,
                        'completions': completions,
                        'labels': labels,
                        'source': source,
                        # 'other_info': other_info,
                        'index': index
                    })
                elif target == 1:
                    code1 = i_detail['code_before']
                    code2 = i_detail['code']
                    code1_blocks, code2_blocks = compare_code_blocks(code1, code2)
                    code1_blocks, code2_blocks = postprocess_code_blocks(code1_blocks, code2_blocks)
                    prompt = 'Determine whether the code is vulnerable or not.'
                    
                    # Process code1 (vulnerable version)
                    completion1 = []
                    labels1 = []
                    for block in code1_blocks:
                        if block['type'] == 'diff':
                            completion1.extend(block['content'].split('\n\n'))
                            labels1.extend([0] * len(block['content'].split('\n\n')))  # 0 = vulnerable/bad
                        else:
                            completion1.extend(block['content'].split('\n\n'))
                            labels1.extend([1] * len(block['content'].split('\n\n')))  # 1 = safe/good
                    
                    # Process code2 (fixed version)
                    completion2 = []
                    labels2 = []
                    for block in code2_blocks:
                        if block['type'] == 'diff':
                            completion2.extend(block['content'].split('\n\n'))
                            labels2.extend([1] * len(block['content'].split('\n\n')))
                        else:
                            completion2.extend(block['content'].split('\n\n'))
                            labels2.extend([1] * len(block['content'].split('\n\n')))  # 1 = safe/good

                    source = 'REPOSVUL'
                    # other_info = { k: data[k] for k in data.keys() }
                    
                    # Add both versions
                    split_data.append({
                        'prompt': prompt,
                        'completions': completion1,
                        'labels': labels1,
                        'source': source,
                        # 'other_info': other_info,
                        'index': index
                    })
                    split_data.append({
                        'prompt': prompt,
                        'completions': completion2,
                        'labels': labels2,
                        'source': source,
                        # 'other_info': other_info,
                        'index': index
                    })
        
        # Convert list to Dataset and add to DatasetDict
        number = len(split_data)
        train_data.extend(split_data[:int(number*0.8)])
        test_data.extend(split_data[int(number*0.8):])
    vul_dataset['train'] = Dataset.from_list(train_data)
    vul_dataset['test'] = Dataset.from_list(test_data)
    print(f"Completed {split_name} split: {len(train_data)} examples")
    print(f"Completed {split_name} test split: {len(test_data)} examples")
    
    # Save all splits together as one DatasetDict (most efficient for datasets library)
    vul_dataset.save_to_disk(path)
    
def concat_eval_dataset():
    data_path = '/project/flame/wyu3/PRM/reposvul_processed_dataset'
    dataset = load_from_disk(data_path)
    eval_dataset = dataset['test']
    train_dataset = dataset['train']
    number = 0
    
    for item in eval_dataset:
        completions = item['completions']
        for completion in completions:
            if completion == '':
                number += 1
                completions.remove(completion)
        new_list = [completions[i] + completions[i + 1] if i + 1 < len(completions) else completions[i] for i in range(0, len(completions), 2)]
        item['completions'] = new_list
    dataset['test'] = eval_dataset
    
    for item in train_dataset:
        completions = item['completions']
        for completion in completions:
            if completion == '':
                number += 1
                completions.remove(completion)
        new_list = [completions[i] + completions[i + 1] if i + 1 < len(completions) else completions[i] for i in range(0, len(completions), 2)]
        item['completions'] = new_list
    dataset['train'] = train_dataset
    
    dataset.save_to_disk(data_path+'_concat')
    print(f"Removed {number} single empty completion")

def reorg():
    from datasets import load_from_disk, DatasetDict

    # 加载数据
    train_data = load_from_disk('/project/flame/wyu3/PRM/all_processed_dataset_31340_tokenized')
    test_data = load_from_disk('/project/flame/wyu3/PRM/all_processed_dataset_31340_tokenized_eval')

    # 合并为一个 DatasetDict
    dataset = DatasetDict({
        'train': train_data,
        'test': test_data
    })

    # 查看结构
    print(dataset)
    dataset.save_to_disk('/project/flame/wyu3/PRM/all_processed_dataset_31340_tokenized_train_test')
    
if __name__ == "__main__":
    path = '/project/flame/wyu3/PRM/reposvul_processed_dataset_merged'
    main(path)
    # concat_eval_dataset()
    # reorg()
   