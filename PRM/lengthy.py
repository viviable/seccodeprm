"""Utility routines for quick dataset inspections."""

from typing import Dict, Iterable, List, Sequence, Tuple
from transformers import AutoTokenizer
import matplotlib.pyplot as plt
from datasets import load_dataset, Dataset


def last_zero_length_ratios(
    dataset: Dataset,
    output_path_last: str = "lengthy.png",
    output_path_first: str = "lengthy_first.png",
) -> Dict[str, List[float]]:
    """
    Compute how early we can stop reading once we hit the first or last 0 label.

    For every example containing at least one 0:
    - locate the first and last index whose label is 0
    - compute the length ratio: cumulative length up to that index (inclusive)
      divided by the total length of all completions
    Two histograms are saved (first/last), and both ratio lists are returned.
    """
    last_ratios: List[float] = []
    last_ratios_tokens: List[float] = []
    first_ratios: List[float] = []
    first_ratios_tokens: List[float] = []
    tokenizer = AutoTokenizer.from_pretrained("meta-llama/Meta-Llama-3-8B-Instruct")

    for example in dataset:
        completions: Sequence[str] = example.get("completions", [])
        labels: Sequence[int] = example.get("labels", [])

        if not completions or not labels or 0 not in labels:
            continue
        last_zero_idx = len(labels) - 1 - list(reversed(labels)).index(0)
        first_zero_idx = list(labels).index(0)

        total_length = sum(len(comp) for comp in completions)
        if total_length == 0:
            continue

        last_used = sum(len(comp) for comp in completions[: last_zero_idx + 1])
        last_used_tokens = tokenizer.encode(''.join(completions[: last_zero_idx + 1]))
        first_used = sum(len(comp) for comp in completions[: first_zero_idx + 1])
        first_used_tokens = tokenizer.encode(''.join(completions[: first_zero_idx + 1]))
        full_tokens = tokenizer.encode(''.join(completions))

        last_ratios.append(last_used / total_length)
        first_ratios.append(first_used / total_length)
        last_ratios_tokens.append(len(last_used_tokens) / len(full_tokens))
        first_ratios_tokens.append(len(first_used_tokens) / len(full_tokens))
    

    return last_ratios, first_ratios, last_ratios_tokens, first_ratios_tokens


def plot_last_zero_length_comparison(
    last_ratios: List[float], first_ratios: List[float], last_ratios_tokens: List[float], first_ratios_tokens: List[float], output_prefix: str = "last_zero_length_compare"
) -> Dict[str, float]:
    """
    Compare the length of the completion at the last 0 with the longest
    completion before it. Produces:
    - box plot for target lengths vs. max previous lengths
    - histogram of target/max_previous ratios (where previous exists)

    Returns a small summary dict for quick reference.
    """
    
    plt.figure(figsize=(6, 4))

    data = [last_ratios, last_ratios_tokens, first_ratios, first_ratios_tokens]
    labels = ["char (last 0)", "token (last 0)", "char (first 0)", "token (first 0)"]

    # 自定义颜色
    colors = ['#4fbd81', '#ade0b4', '#5dbfe9', '#e7e6d4']

    bp = plt.boxplot(
        data,
        tick_labels=labels,
        showfliers=False,
        patch_artist=True,  # 关键：允许填充颜色
        
    )

    # 设置每个箱体的颜色
    for patch, color in zip(bp['boxes'], colors):
        patch.set_facecolor(color)
        patch.set_alpha(0.95)

    # 设置中位线颜色和粗细
    for median in bp['medians']:
        median.set_color('black')
        median.set_linewidth(2)

    plt.ylabel("Ratio", fontsize=20)
    # plt.xlabel("Length Type", fontsize=20)
    plt.xticks(fontsize=12)
    plt.yticks(fontsize=18)
    plt.title("Needed Length Ratio (char, token)", fontsize=18)
    plt.tight_layout()
    plt.savefig(f"{output_prefix}_box.png", dpi=150)
    plt.close()
        

if __name__ == "__main__":
    dataset = load_dataset('vivi-yu/primevul_processed_dataset')['test']
    last_ratios, first_ratios, last_ratios_tokens, first_ratios_tokens = last_zero_length_ratios(dataset)
    plot_last_zero_length_comparison(last_ratios, first_ratios, last_ratios_tokens, first_ratios_tokens)
