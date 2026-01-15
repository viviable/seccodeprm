from datasets import load_dataset, DatasetDict, Dataset, load_from_disk
from tqdm import tqdm
import json
import re
import ast
import os

from difflib import SequenceMatcher
from typing import List, Tuple

PARSE_STATS = {
    "ast_attempts": 0,
    "ast_failures": 0,
    "clang_attempts": 0,
    "clang_failures": 0,
    "samples": 0,
    "samples_with_zero_label_blocks": 0,
}

try:
    from clang import cindex as clang_cindex
    _libclang_file = os.environ.get("LIBCLANG_FILE")
    _libclang_path = os.environ.get("LIBCLANG_PATH")
    _default_libclang = "/lib/x86_64-linux-gnu/libclang-14.so.13"
    if _libclang_file:
        clang_cindex.Config.set_library_file(_libclang_file)
    elif _libclang_path:
        clang_cindex.Config.set_library_path(_libclang_path)
    elif os.path.exists(_default_libclang):
        clang_cindex.Config.set_library_file(_default_libclang)
except Exception:
    clang_cindex = None

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
    i = 0
    while i < len(list2):
        block = list2[i]
        if '/*' in block['content'] and '*/' not in block['content']:
            # find the next block that contains '*/'
            for j in range(i + 1, len(list2)):
                if '*/' in list2[j]['content']:
                    list2[i]['content'] += ''.join(
                        [block['content'] for block in list2[i + 1 : j + 1]]
                    )
                    list1[i]['content'] += ''.join(
                        [block['content'] for block in list1[i + 1 : j + 1]]
                    )
                    del list2[i + 1 : j + 1]
                    del list1[i + 1 : j + 1]
                    break
        i += 1
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

def extract_function_name_from_block(block):
    python_def = re.search(r'^\s*def\s+([A-Za-z_]\w*)\s*\(', block, re.MULTILINE)
    if python_def:
        return python_def.group(1)

    c_like_def = re.search(
        r'^\s*(?:[\w:\<\>\~\*&]+\s+)+([A-Za-z_]\w*)\s*\([^;]*\)\s*\{',
        block,
        re.MULTILINE,
    )
    if c_like_def:
        return c_like_def.group(1)

    # java_like_def = re.search(
    #     r'^\s*(?:public|private|protected|static|final|native|synchronized|abstract|'
    #     r'transient|strictfp|\s)+\s*([A-Za-z_]\w*)\s*\([^;]*\)\s*\{',
    #     block,
    #     re.MULTILINE,
    # )
    # if java_like_def:
    #     return java_like_def.group(1)

    return None


def is_function_called_in_code(code: str, func_name: str) -> bool:
    try:
        tree = ast.parse(code)
    except SyntaxError:
        return False

    class CallVisitor(ast.NodeVisitor):
        def __init__(self):
            self.called = False

        def visit_Call(self, node):
            if self.called:
                return
            func = node.func
            if isinstance(func, ast.Name) and func.id == func_name:
                self.called = True
                return
            if isinstance(func, ast.Attribute) and func.attr == func_name:
                self.called = True
                return
            self.generic_visit(node)

    visitor = CallVisitor()
    visitor.visit(tree)
    return visitor.called


def is_function_called_in_completions(completions, func_name: str) -> bool:
    code = "\n\n".join([c for c in completions if c])
    return is_function_called_in_code(code, func_name)


def _build_step_line_ranges(completions):
    line_ranges = []
    current_start = 1
    for completion in completions:
        if not completion:
            line_ranges.append((current_start, current_start - 1))
            continue
        line_count = completion.count("\n") + 1
        line_ranges.append((current_start, current_start + line_count - 1))
        current_start += line_count + 2  # account for the "\n\n" joiner
    return line_ranges


def _line_to_step(line_ranges, lineno):
    for idx, (start, end) in enumerate(line_ranges):
        if start <= lineno <= end:
            return idx
    return None


def _c_has_function_definition(block, func_name: str) -> bool:
    if not func_name:
        return False
    pattern = (
        r'^\s*(?:[\w:\<\>\~\*&]+\s+)+'
        + re.escape(func_name)
        + r'\s*\([^;]*\)\s*\{'
    )
    return re.search(pattern, block, re.MULTILINE) is not None


def _c_has_function_call(block, func_name: str) -> bool:
    if not func_name:
        return False
    call_pattern = r'\b' + re.escape(func_name) + r'\s*\('
    if not re.search(call_pattern, block):
        return False
    if _c_has_function_definition(block, func_name):
        return len(re.findall(call_pattern, block)) > 1
    return True


def _c_steps_calling_function(completions, func_name: str):
    if not func_name:
        return []
    steps = []
    for idx, block in enumerate(completions):
        if block and _c_has_function_call(block, func_name):
            steps.append(idx)
    return steps


def _c_steps_called_by_function(completions, func_name: str):
    if not func_name:
        return []
    def_step = None
    for idx, block in enumerate(completions):
        if block and _c_has_function_definition(block, func_name):
            def_step = idx
            break
    if def_step is None:
        return []

    steps_in_function = []
    brace_balance = 0
    for idx in range(def_step, len(completions)):
        block = completions[idx] or ""
        brace_balance += block.count("{")
        brace_balance -= block.count("}")
        steps_in_function.append(idx)
        if brace_balance <= 0 and idx > def_step:
            break

    called_steps = set()
    for idx in steps_in_function:
        block = completions[idx] or ""
        if _c_has_function_call(block, func_name):
            continue
        if re.search(r'\b[A-Za-z_]\w*\s*\(', block):
            called_steps.add(idx)
    return sorted(called_steps)


def _clang_parse(code: str):
    if clang_cindex is None:
        return None
    try:
        index = clang_cindex.Index.create()
    except Exception:
        return None
    try:
        return index.parse(
            "temp.c",
            args=["-xc", "-std=c11"],
            unsaved_files=[("temp.c", code)],
            options=0,
        )
    except Exception:
        return None


def _clang_call_name(cursor):
    if cursor.spelling:
        return cursor.spelling
    if cursor.referenced is not None and cursor.referenced.spelling:
        return cursor.referenced.spelling
    if cursor.displayname:
        return cursor.displayname.split("(")[0]
    return ""


def _clang_steps_calling_function(completions, func_name: str):
    if not func_name:
        return []
    steps = set()
    for idx, block in enumerate(completions):
        if not block:
            continue
        PARSE_STATS["clang_attempts"] += 1
        tu = _clang_parse(block)
        if tu is None:
            PARSE_STATS["clang_failures"] += 1
            if _c_has_function_call(block, func_name):
                steps.add(idx)
            continue
        for cursor in tu.cursor.walk_preorder():
            if cursor.kind == clang_cindex.CursorKind.CALL_EXPR:
                if _clang_call_name(cursor) == func_name:
                    steps.add(idx)
                    break
    return sorted(steps)


def _clang_steps_called_by_function(completions, func_name: str):
    if not func_name:
        return []
    for idx, block in enumerate(completions):
        if not block or not _c_has_function_definition(block, func_name):
            continue
        PARSE_STATS["clang_attempts"] += 1
        tu = _clang_parse(block)
        if tu is None:
            PARSE_STATS["clang_failures"] += 1
            return _c_steps_called_by_function(completions, func_name)
        for cursor in tu.cursor.walk_preorder():
            if cursor.kind == clang_cindex.CursorKind.FUNCTION_DECL and cursor.spelling == func_name:
                for child in cursor.walk_preorder():
                    if child.kind == clang_cindex.CursorKind.CALL_EXPR:
                        return [idx]
                return []
    return []


def find_steps_calling_function(completions, func_name: str):
    code = "\n\n".join([c for c in completions if c])
    try:
        PARSE_STATS["ast_attempts"] += 1
        tree = ast.parse(code)
    except SyntaxError:
        PARSE_STATS["ast_failures"] += 1
        return _clang_steps_calling_function(completions, func_name)
    line_ranges = _build_step_line_ranges(completions)
    steps = set()

    class CallStepVisitor(ast.NodeVisitor):
        def visit_Call(self, node):
            func = node.func
            if isinstance(func, ast.Name) and func.id == func_name:
                step = _line_to_step(line_ranges, node.lineno)
                if step is not None:
                    steps.add(step)
            elif isinstance(func, ast.Attribute) and func.attr == func_name:
                step = _line_to_step(line_ranges, node.lineno)
                if step is not None:
                    steps.add(step)
            self.generic_visit(node)

    CallStepVisitor().visit(tree)
    return sorted(steps)


def find_steps_called_by_function(completions, func_name: str):
    code = "\n\n".join([c for c in completions if c])
    try:
        PARSE_STATS["ast_attempts"] += 1
        tree = ast.parse(code)
    except SyntaxError:
        PARSE_STATS["ast_failures"] += 1
        return _clang_steps_called_by_function(completions, func_name)
    line_ranges = _build_step_line_ranges(completions)
    steps = set()

    class FuncCallVisitor(ast.NodeVisitor):
        def __init__(self):
            self.in_target = False

        def visit_FunctionDef(self, node):
            if node.name != func_name:
                return
            self.in_target = True
            self.generic_visit(node)
            self.in_target = False

        def visit_Call(self, node):
            if self.in_target:
                step = _line_to_step(line_ranges, node.lineno)
                if step is not None:
                    steps.add(step)
            self.generic_visit(node)

    FuncCallVisitor().visit(tree)
    return sorted(steps)


def find_step_function_names(step_blocks, all_completions):
    results = []
    seen_function_names = set()
    for step, block in step_blocks:
        function_name = extract_function_name_from_block(block)
        if function_name is None:
            for prev_step in range(step - 1, -1, -1):
                prev_block = all_completions[prev_step]
                function_name = extract_function_name_from_block(prev_block)
                if function_name is not None:
                    break
        if function_name in seen_function_names:
            continue
        seen_function_names.add(function_name)
        results.append({
            'step': step,
            'function_name': function_name,
            'completion': block,
        })
    return results




def bump_labels_for_used_steps(completions, labels, bump=0.5):
    PARSE_STATS["samples"] += 1
    zero_label_blocks = []
    for i, (completion, label) in enumerate(zip(completions, labels)):
        if label == 0 and completion != '':
            zero_label_blocks.append((i, completion))

    if not zero_label_blocks:
        return labels
    PARSE_STATS["samples_with_zero_label_blocks"] += 1

    results = find_step_function_names(zero_label_blocks, completions)
    updated_labels = labels[:]
    used_steps = set()
    for result in results:
        steps_calling_function = find_steps_calling_function(completions, result['function_name'])
        steps_called_by_function = find_steps_called_by_function(completions, result['function_name'])
        used_steps.update(steps_calling_function)
        used_steps.update(steps_called_by_function)

    for step in used_steps:
        if 0 <= step < len(updated_labels):
            updated_labels[step] -= bump

    return updated_labels

def relabel_dataset(dataset_train):
    def _update(example):
        example['labels'] = bump_labels_for_used_steps(
            example['completions'], example['labels']
        )
        return example

    updated = dataset_train.map(_update)
    if PARSE_STATS["ast_attempts"] > 0:
        ast_fail_rate = PARSE_STATS["ast_failures"] / PARSE_STATS["ast_attempts"]
    else:
        ast_fail_rate = 0.0
    if PARSE_STATS["clang_attempts"] > 0:
        clang_fail_rate = PARSE_STATS["clang_failures"] / PARSE_STATS["clang_attempts"]
    else:
        clang_fail_rate = 0.0
    print(
        "parse_stats:",
        f"ast_fail={PARSE_STATS['ast_failures']}/{PARSE_STATS['ast_attempts']} ({ast_fail_rate:.3f})",
        f"clang_fail={PARSE_STATS['clang_failures']}/{PARSE_STATS['clang_attempts']} ({clang_fail_rate:.3f})",
        f"samples={PARSE_STATS['samples']}",
        f"samples_with_zero_label_blocks={PARSE_STATS['samples_with_zero_label_blocks']}",
    )
    return updated
    
def concat_eval_dataset():
    data_path = '/project/flame/wyu3/PRM/reposvul_processed_dataset'
    dataset = load_from_disk(data_path)
    eval_dataset = dataset['test']
    train_dataset = dataset['train']
    number = 0
    
    for item in eval_dataset:
        completions = item['completions']
        empty_count = sum(1 for completion in completions if completion == '')
        if empty_count:
            number += empty_count
            completions = [completion for completion in completions if completion != '']
        new_list = [completions[i] + completions[i + 1] if i + 1 < len(completions) else completions[i] for i in range(0, len(completions), 2)]
        item['completions'] = new_list
    dataset['test'] = eval_dataset
    
    for item in train_dataset:
        completions = item['completions']
        empty_count = sum(1 for completion in completions if completion == '')
        if empty_count:
            number += empty_count
            completions = [completion for completion in completions if completion != '']
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
    path = 'vivi-yu/reposvul_processed_dataset'
    old_dataset = load_dataset(path)
    new_dataset = DatasetDict({
        'train': relabel_dataset(old_dataset['train']),
        'test': old_dataset['test']
    })
    import pdb; pdb.set_trace()
    new_dataset.save_to_disk('/project/flame/wyu3/PRM/reposvul_processed_dataset_relabeled')
    # main(path)
    # concat_eval_dataset()
    # reorg()
   
