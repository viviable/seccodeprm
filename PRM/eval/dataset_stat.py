from datasets import load_dataset
from transformers import AutoTokenizer

import json
import re

datasets = {
    "precisebugs_test": load_dataset("vivi-yu/vul_code_precise")["test"],
    "reposvul_test": load_dataset("vivi-yu/reposvul_processed_dataset")["test"],
    "sven_test": load_dataset("vivi-yu/vul_code_sven")["val"],
    "bigvul_dedup_test": load_dataset("vivi-yu/bigvul_dedup_test")["train"],
    "primevul_test_paired": load_dataset("vivi-yu/primevul_processed_dataset")["test"],
    "primevul_test_unpaired": load_dataset("vivi-yu/primevul_processed_dataset_unpaired")["test"],
}


def extract_language(data_dict):
    """从 CVE 数据中提取编程语言"""
    
    # 方法 1: 从 message 提取
    message = data_dict.get('message', '')
    
    # 常见语言关键词
    language_keywords = {
        'php': 'PHP',
        'python': 'Python',
        'java': 'Java',
        'javascript': 'JavaScript',
        'typescript': 'TypeScript',
        'c++': 'C++',
        'cpp': 'Cpp',
        'c#': 'C#',
        'ruby': 'Ruby',
        'go': 'Go',
        'rust': 'Rust',
        'node.js': 'Node.js',
        'node': 'Node.js',
    }
    
    # 在 message 中搜索
    message_lower = message.lower()
    for keyword, lang in language_keywords.items():
        if keyword in message_lower:
            return lang
    
    # 方法 2: 从 descriptions 提取
    descriptions = data_dict.get('other', {}).get('cve', {}).get('descriptions', [])
    for desc in descriptions:
        desc_value = desc.get('value', '').lower()
        for keyword, lang in language_keywords.items():
            if keyword in desc_value:
                return lang
    
    # 方法 3: 从 CPE 或产品名称推断
    # CodeIgniter -> PHP
    # Django -> Python
    # Spring -> Java
    # 等等
    
    return None

def extract_language_from_file_name(file_name):
    
    if ".cpp" in file_name:
        return "Cpp"
    elif ".java" in file_name:
        return "Java"
    elif ".javascript" in file_name:
        return "JavaScript"
    elif ".typescript" in file_name:
        return "TypeScript"
    elif ".c#" in file_name:
        return "C#"
    elif ".py" in file_name:
        return "Python"
    elif ".ruby" in file_name:
        return "Ruby"
    elif ".go" in file_name:
        return "Go"
    elif ".rust" in file_name:
        return "Rust"
    elif ".node.js" in file_name:
        return "Node.js"
    elif ".node" in file_name:
        return "Node.js"
    elif ".c" in file_name:
        return "C"
    else:
        print(f"unknown language: {file_name}")
        return None
    
def get_dataset_stat(dataset_name):
    dataset = datasets[dataset_name]
    ############ sample level stats ############
    ## vul and safe
    vul = [example for example in dataset if 0 in example['labels']]
    safe = [example for example in dataset if 0 not in example['labels']]
    print(f"Dataset {dataset_name} has {len(vul)} vul and {len(safe)} safe examples, ratio is {len(vul) / (len(vul) + len(safe))}")
    
    ## token length
    tokenizer = AutoTokenizer.from_pretrained("Qwen/Qwen2.5-Coder-7B-Instruct")
    vul_token_lengths = [len(tokenizer.encode('\n\n'.join(example['completions']))) for example in vul]
    safe_token_lengths = [len(tokenizer.encode('\n\n'.join(example['completions']))) for example in safe]
    print(f"average vul token length is {sum(vul_token_lengths) / len(vul)}, average safe token length is {sum(safe_token_lengths) / len(safe)}")
    print(f"average all token length {(sum(vul_token_lengths)+sum(safe_token_lengths)) / (len(vul) + len(safe))}")
    
    ## char length
    vul_char_lengths = [len('\n\n'.join(example['completions'])) for example in vul]
    safe_char_lengths = [len('\n\n'.join(example['completions'])) for example in safe]
    print(f"average vul char length is {sum(vul_char_lengths) / len(vul_char_lengths)}, average safe char length is {sum(safe_char_lengths) / len(safe_char_lengths)}")
    print(f"average all char length {(sum(vul_char_lengths)+sum(safe_char_lengths)) / (len(vul) + len(safe))}")
    
    ############ step level stats ############
    ## number of steps
    vul_num_steps = [len(example['completions']) for example in vul]
    safe_num_steps = [len(example['completions']) for example in safe]
    print(f"average vul num steps is {sum(vul_num_steps) / len(vul_num_steps)}, average safe num steps is {sum(safe_num_steps) / len(safe_num_steps)}")
    print(f"average all num steps {(sum(vul_num_steps)+sum(safe_num_steps)) / (len(vul) + len(safe))}")
    
    ## step level ratios
    vul_step_num = [len(example['labels'])-sum(example['labels']) for example in vul]  ### vul steps num
    vul_step_ratios = [1-sum(example['labels'])/len(example['labels']) for example in vul]  ### vul steps ratios
    total_step_num = [len(example['labels']) for example in dataset]
    
    print(f"average vul step ratio in vul examples is {sum(vul_step_ratios) / len(vul_step_ratios)}")
    print(f"average vul step in all examples is {sum(vul_step_num) / sum(total_step_num)}")
    
    import pdb; pdb.set_trace()
    ### cwe  and language type
    
    if dataset_name == "precisebugs_test":
        vul_cwes = []
        vul_language_types = []
        for example in dataset:
            data = example['other_info']
            data_dict = json.loads(data)
            cwe = data_dict['type']
            language = extract_language(data_dict)
            vul_cwes.append(cwe)
            vul_language_types.append(language)
    elif dataset_name == "reposvul_test":
        vul_cwes = []
        vul_language_types = []
    elif dataset_name == "sven_test":
        vul_cwes = []
        vul_language_types = []
        for example in dataset:
            data = example['other_info']
            cwe = data['vul_type']
            vul_cwes.append(cwe)
    elif dataset_name == "bigvul_dedup_test":
        vul_cwes = []
        vul_language_types = []
        for example in dataset:
            data = example['other_info']
            cwe = data['CWE ID']
            language = data['lang']
            vul_cwes.append(cwe)
            vul_language_types.append(language)
    elif dataset_name == "primevul_test_paired":
        vul_cwes = []
        vul_language_types = []
        for example in dataset:
            data = example['other_info']
            cwe = data['cwe']
            language = extract_language_from_file_name(data['file_name'])
            vul_cwes.extend(cwe)
            vul_language_types.append(language)
    elif dataset_name == "primevul_test_unpaired":
        vul_cwes = []
        vul_language_types = []
        for example in dataset:
            data = example['other_info']
            cwe = data['cwe']
            language = extract_language_from_file_name(data['file_name'])
            vul_cwes.extend(cwe)
            vul_language_types.append(language)
    import pdb; pdb.set_trace()
    print(f"vul cwes: {set(vul_cwes)}")
    print(f"vul language types: {set(vul_language_types)}")
    
    
    
if __name__ == "__main__":
    # get_dataset_stat("precisebugs_test")
    # get_dataset_stat("reposvul_test")
    # get_dataset_stat("sven_test")
    # get_dataset_stat("bigvul_dedup_test")
    get_dataset_stat("primevul_test_paired")
    get_dataset_stat("primevul_test_unpaired")
    
    