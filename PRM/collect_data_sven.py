from datasets import load_dataset, DatasetDict, Dataset, load_from_disk
from tqdm import tqdm
import os
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

def load_data(split_name):
    json_path = f'/home/wyu3/workspace/sven/data_train_val/{split_name}'
    vul_list = [22, 78, 79, 89, 125, 190, 416, 476, 787]
    data = []
    for vul in vul_list:
        file_name = f'cwe-{vul:03d}.jsonl'
        with open(os.path.join(json_path, file_name), 'r') as f:
            data.extend([json.loads(line) for line in f])
    # turn data into Dataset
    dataset = Dataset.from_list(data)
    return dataset

def main(path):
    ## 
    # Create DatasetDict once to hold all splits
    vul_dataset = DatasetDict()
    
    # Process each split
    splits = ['train','val']
    for split_name in splits:
        print(f"Processing {split_name} split...")
        dataset = load_data(split_name)
        split_data = []
        
        for index, data in enumerate(tqdm(dataset, desc=f"Processing {split_name}")):
            # if data['file_name'].endswith('.py'):
            #     continue
            code1 = data['func_src_before']
            code2 = data['func_src_after']
            code1_blocks, code2_blocks = compare_code_blocks(code1, code2)
            prompt = 'Determine whether the {function_name} code is vulnerable or not.'
            
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
            if not 0 in labels1:
                labels1[-1] = 0
            
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

            source = 'SVEN'
            other_info = { k: data[k] for k in data.keys() if k not in ['func_src_before', 'func_src_after'] }
            
            # Add both versions
            split_data.append({
                'prompt': prompt,
                'completions': completion1,
                'labels': labels1,
                'source': source,
                'other_info': other_info,
                'index': index
            })
            split_data.append({
                'prompt': prompt,
                'completions': completion2,
                'labels': labels2,
                'source': source,
                'other_info': other_info,
                'index': index
            })
        
        # Convert list to Dataset and add to DatasetDict
        vul_dataset[split_name] = Dataset.from_list(split_data)
        print(f"Completed {split_name} split: {len(split_data)} examples")
    
    # Save all splits together as one DatasetDict (most efficient for datasets library)
    vul_dataset.save_to_disk(path)
    
    

if __name__ == "__main__":
    path = './sven_processed_dataset'
    main(path)
    # path = '/project/flame/wyu3/PRM/bigvul_processed_dataset_one_zero'
    # one_zero_dataset(path)