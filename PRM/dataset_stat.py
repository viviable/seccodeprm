import json
import re
from collections import Counter
from pathlib import Path

from datasets import load_dataset, load_from_disk
from transformers import AutoTokenizer
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
## for test
# datasets = {
#     "precisebugs_test": load_dataset("vivi-yu/vul_code_precise")["test"],
#     "reposvul_test": load_dataset("vivi-yu/reposvul_processed_dataset")["test"],
#     "sven_test": load_dataset("vivi-yu/vul_code_sven")["val"],
#     "bigvul_dedup_test": load_dataset("vivi-yu/bigvul_dedup_test")["train"],
#     "primevul_test_paired": load_dataset("vivi-yu/primevul_processed_dataset")["test"],
#     "primevul_test_unpaired": load_dataset("vivi-yu/primevul_processed_dataset_unpaired")["test"],
# }
# for train
datasets = {
    # "all_train": load_dataset("vivi-yu/vul_code_precise")["train"],
    "precisebugs_train": load_dataset("vivi-yu/vul_code_precise")["train"], 
    # "precisebugs_train": load_from_disk("/project/flame/wyu3/PRM/precisebugs_processed_dataset")["train"], 
    # "reposvul_train_func": load_from_disk("/project/flame/wyu3/PRM/reposvul_processed_dataset_func")["train"],
    
    "reposvul_train": load_dataset("vivi-yu/reposvul_processed_dataset")["train"],
    "reposvul_train_cleaned": load_from_disk("/project/flame/wyu3/PRM/reposvul_processed_dataset_cleaned")["train"],
    "sven_train": load_dataset("vivi-yu/vul_code_sven")["train"],
    # "bigvul_train": load_dataset("vivi-yu/bigvul_processed")["train"],
    "primevul_train_paired": load_dataset("vivi-yu/primevul_processed_dataset")["train"],
    # "primevul_train_unpaired": load_dataset("vivi-yu/primevul_processed_dataset_unpaired")["train"],
}

test_datasets = {
    "precisebugs_test": load_dataset("vivi-yu/vul_code_precise")["test"],
    # "reposvul_test_func": load_from_disk("/project/flame/wyu3/PRM/reposvul_processed_dataset_func")["test"],
    "reposvul_test": load_dataset("vivi-yu/reposvul_processed_dataset")["test"],
    "reposvul_test_cleaned": load_from_disk("/project/flame/wyu3/PRM/reposvul_processed_dataset_cleaned")["test"],
    "sven_test": load_dataset("vivi-yu/vul_code_sven")["val"],
    # "bigvul_dedup_test": load_dataset("vivi-yu/bigvul_dedup_test")["train"],
    "primevul_test_paired": load_dataset("vivi-yu/primevul_processed_dataset")["test"],
    # "primevul_test_unpaired": load_dataset("vivi-yu/primevul_processed_dataset_unpaired")["test"],
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


def collect_cwe_and_language_counts(dataset_name):
    """Collect CWE and language counts for the given dataset name."""
    dataset = datasets.get(dataset_name) or test_datasets.get(dataset_name)
    if dataset is None:
        raise ValueError(f"Unknown dataset: {dataset_name}")

    cwe_counter = Counter()
    language_counter = Counter()

    for example in dataset:
        if "precisebugs" in dataset_name or "reposvul" in dataset_name:
            cwe = example.get("cwe")
            language = example.get("language")
        elif "sven" in dataset_name:
            info = example.get("other_info", {})
            cwe = info.get("vul_type")
            language = info.get("lang") or info.get("language")
        elif "bigvul" in dataset_name:
            info = example.get("other_info", {})
            cwe = info.get("CWE ID") or info.get("cwe")
            language = info.get("lang") or info.get("language")
        elif "primevul" in dataset_name:
            info = example.get("other_info", {})
            cwes = info.get("cwe") or []
            for cwe_item in cwes:
                if cwe_item:
                    cwe_counter[cwe_item] += 1
            file_name = info.get("file_name", "")
            language = extract_language_from_file_name(file_name)
            cwe = None
        else:
            cwe = None
            language = None

        if cwe:
            cwe_counter[cwe] += 1
        if language:
            language_counter[language] += 1

    return cwe_counter, language_counter


def _plot_bar(counter, title, output_path, xlabel="Number of examples", max_items=12):
    """Draw a horizontal bar chart from a Counter and save it."""
    if not counter:
        print(f"No data to plot for {title}")
        return None

    top_items = counter.most_common(max_items)
    labels = [item[0] if item[0] else "Unknown" for item in top_items]
    counts = [item[1] for item in top_items]
    y_pos = np.arange(len(labels))

    fig_height = max(3, len(labels) * 0.45)
    fig, ax = plt.subplots(figsize=(8, fig_height))
    colors = plt.cm.Paired(np.linspace(0, 1, len(labels)))

    ax.barh(y_pos, counts, color=colors, edgecolor="white")
    ax.set_yticks(y_pos)
    ax.set_yticklabels([f"{label} ({count})" for label, count in top_items])
    ax.invert_yaxis()
    ax.set_xlabel(xlabel)
    ax.set_title(title)

    max_count = max(counts)
    for idx, count in enumerate(counts):
        ax.text(count + max_count * 0.01, idx, str(count), va="center", fontsize=9)

    plt.tight_layout()
    plt.savefig(output_path, dpi=200)
    plt.close(fig)
    print(f"Saved bar chart to {output_path}")
    return output_path


def plot_language_and_cwe_bars(dataset_name, max_items=12, save_dir="."):
    """
    Draw two bar figures (language and CWE) for a dataset.

    Args:
        dataset_name: Key in datasets or test_datasets.
        max_items: Only plot the top-N most common items.
        save_dir: Directory to save the figures.
    """
    cwe_counts, language_counts = collect_cwe_and_language_counts(dataset_name)
    save_path = Path(save_dir)
    save_path.mkdir(parents=True, exist_ok=True)

    _plot_bar(
        language_counts,
        f"{dataset_name} language distribution",
        save_path / f"{dataset_name}_language_bar.png",
        max_items=max_items,
    )
    _plot_bar(
        cwe_counts,
        f"{dataset_name} CWE distribution",
        save_path / f"{dataset_name}_cwe_bar.png",
        max_items=max_items,
    )
    
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
    
    ### cwe  and language type
    if 'precisebugs' in dataset_name:
        vul_cwes = {}
        vul_language_types = {}
        for example in dataset:
            
            cwe = example['cwe']
            language = example['language']
            vul_cwes[cwe] = vul_cwes.get(cwe, 0) + 1
            vul_language_types[language] = vul_language_types.get(language, 0) + 1
    elif 'reposvul' in dataset_name:
        
        vul_cwes = {}
        vul_language_types = {}
        for example in dataset:
            cwe = example['cwe']
            language = example['language']
            vul_cwes[cwe] = vul_cwes.get(cwe, 0) + 1
            vul_language_types[language] = vul_language_types.get(language, 0) + 1
    elif 'sven' in dataset_name:
        vul_cwes = []
        vul_language_types = []
        for example in dataset:
            data = example['other_info']
            cwe = data['vul_type']
            vul_cwes.append(cwe)
    elif 'bigvul' in dataset_name:
        vul_cwes = []
        vul_language_types = []
        for example in dataset:
            data = example['other_info']
            cwe = data['CWE ID']
            language = data['lang']
            vul_cwes.append(cwe)
            vul_language_types.append(language)
    elif 'primevul' in dataset_name:
        vul_cwes = []
        vul_language_types = []
        for example in dataset:
            data = example['other_info']
            cwe = data['cwe']
            language = extract_language_from_file_name(data['file_name'])
            vul_cwes.extend(cwe)
            vul_language_types.append(language)

    print(f"vul cwes: {(vul_cwes)}")
    print(f"vul language types: {(vul_language_types)}")
    

def _token_lengths(dataset, tokenizer, separator="\n\n"):
    """Return token lengths for all examples using the given tokenizer."""
    return [
        len(tokenizer.encode(separator.join(example["completions"]), add_special_tokens=False))
        for example in dataset
    ]


def analyze_train_test_token_lengths(train_name, test_name, separator="\n\n", tokenizer_name="Qwen/Qwen2.5-Coder-7B-Instruct"):
    """Plot and report token length distributions for train/test splits."""
    if train_name not in datasets:
        raise ValueError(f"Unknown train dataset: {train_name}")
    if test_name not in test_datasets:
        raise ValueError(f"Unknown test dataset: {test_name}")

    tokenizer = AutoTokenizer.from_pretrained(tokenizer_name)
    train_lengths = _token_lengths(datasets[train_name], tokenizer, separator)
    test_lengths = _token_lengths(test_datasets[test_name], tokenizer, separator)

    print(f"{train_name} token length avg: {np.mean(train_lengths):.2f}, median: {np.median(train_lengths):.2f}, max: {np.max(train_lengths)}")
    print(f"{test_name} token length avg: {np.mean(test_lengths):.2f}, median: {np.median(test_lengths):.2f}, max: {np.max(test_lengths)}")

    bins = 60
    max_len = max(np.max(train_lengths), np.max(test_lengths))
    fig, ax = plt.subplots(figsize=(9, 4))
    ax.hist(train_lengths, bins=bins, range=(0, max_len), alpha=0.6, label="train", color="#6c8cd5")
    ax.hist(test_lengths, bins=bins, range=(0, max_len), alpha=0.6, label="test", color="#d58c6c")
    ax.set_yscale("log")
    ax.set_xlabel("Token length")
    ax.set_ylabel("Number of examples (log scale)")
    ax.set_title(f"Token length distribution: {train_name} vs {test_name}")
    ax.legend()
    ax.grid(True, axis="y", linestyle="--", alpha=0.4)
    plt.tight_layout()
    output_path = f"{train_name}_vs_{test_name}_token_lengths.png"
    plt.savefig(output_path)
    plt.close(fig)
    print(f"Saved histogram to {output_path}")
    
    
if __name__ == "__main__":
    # get_dataset_stat("precisebugs_train")
    # get_dataset_stat("reposvul_train_func")
    # analyze_train_test_token_lengths("reposvul_train", "reposvul_test")
    get_dataset_stat("reposvul_train_cleaned")
    # get_dataset_stat("sven_train")
    # get_dataset_stat("bigvul_train")
    # get_dataset_stat("primevul_train_paired")
    # get_dataset_stat("primevul_train_unpaired")
    # plot_language_and_cwe_bars("reposvul_train")
    
    
