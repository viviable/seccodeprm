#!/usr/bin/env python3
"""
Analyze core statistics of several Hugging Face datasets used in PRM evaluation.

For each dataset we report:
  • Number of samples.
  • Character/token length statistics for string fields.
  • Step counts for sequence fields such as completions.
  • Language distribution when language-related columns exist.

Example:
    python analyze_datasets.py --tokenizer Qwen/Qwen2.5-Math-PRM-7B --max-samples 2000
"""

from __future__ import annotations

import argparse
import json
import math
from collections import Counter
from dataclasses import dataclass, field
from typing import Dict, Iterable, List, Optional, Sequence, Tuple

import numpy as np
from datasets import Dataset, IterableDataset, load_dataset


DATASETS: Dict[str, Tuple[str, str]] = {
    "precisebugs_test": ("vivi-yu/vul_code_precise", "test"),
    "reposvul_test": ("vivi-yu/reposvul_processed_dataset", "test"),
    "sven_test": ("vivi-yu/vul_code_sven", "val"),
    "bigvul_dedup_test": ("vivi-yu/bigvul_dedup_test", "train"),
    "primevul_test_paired": ("vivi-yu/primevul_processed_dataset", "test"),
    "primevul_test_unpaired": ("vivi-yu/primevul_processed_dataset_unpaired", "test"),
}


@dataclass
class FieldStats:
    """Aggregate statistics for a single textual field."""

    char_lengths: List[int] = field(default_factory=list)
    token_lengths: List[int] = field(default_factory=list)

    def to_summary(self) -> Dict[str, float]:
        return compute_summary(self.char_lengths, self.token_lengths)


@dataclass
class SequenceFieldStats:
    """Aggregate statistics for sequence (list) fields such as completions."""

    step_lengths: List[int] = field(default_factory=list)
    item_char_lengths: List[int] = field(default_factory=list)
    item_token_lengths: List[int] = field(default_factory=list)

    def to_summary(self) -> Dict[str, Dict[str, float]]:
        return {
            "steps": summarize_distribution(self.step_lengths),
            "item_char_lengths": summarize_distribution(self.item_char_lengths),
            "item_token_lengths": summarize_distribution(self.item_token_lengths),
        }


def compute_summary(char_lengths: Sequence[int], token_lengths: Sequence[int]) -> Dict[str, Dict[str, float]]:
    return {
        "char_lengths": summarize_distribution(char_lengths),
        "token_lengths": summarize_distribution(token_lengths),
    }


def summarize_distribution(values: Sequence[int]) -> Dict[str, float]:
    if not values:
        return {}
    arr = np.asarray(values, dtype=np.int64)
    return {
        "count": int(arr.size),
        "mean": float(arr.mean()),
        "median": float(np.median(arr)),
        "p95": float(np.percentile(arr, 95)),
        "max": int(arr.max()),
    }


def detect_fields(dataset: Dataset) -> Tuple[List[str], List[str], List[str]]:
    """Identify textual fields, sequence-of-text fields, and language fields."""
    from datasets.features import Sequence, Value  # imported lazily

    text_fields: List[str] = []
    seq_text_fields: List[str] = []
    language_fields: List[str] = []

    for name, feature in dataset.features.items():
        if isinstance(feature, Value) and feature.dtype == "string":
            text_fields.append(name)
            if name.lower() in {"language", "lang", "programming_language"}:
                language_fields.append(name)
        elif isinstance(feature, Sequence):
            nested = feature.feature
            if hasattr(nested, "dtype") and getattr(nested, "dtype") == "string":
                seq_text_fields.append(name)

    # Heuristics for language columns not marked as string Value.
    for candidate in ("language", "lang", "programming_language"):
        if candidate in dataset.column_names and candidate not in language_fields:
            language_fields.append(candidate)

    return text_fields, seq_text_fields, language_fields


def load_tokenizer(model_name_or_path: Optional[str]):
    if not model_name_or_path:
        return None
    from transformers import AutoTokenizer  # type: ignore

    print(f"[INFO] Loading tokenizer: {model_name_or_path}")
    tokenizer = AutoTokenizer.from_pretrained(model_name_or_path, use_fast=True)
    if tokenizer.pad_token is None:
        tokenizer.pad_token = tokenizer.eos_token
    return tokenizer


def gather_stats(
    dataset_name: str,
    dataset: Dataset,
    tokenizer,
    max_samples: Optional[int],
) -> Dict[str, object]:
    text_fields, seq_text_fields, language_fields = detect_fields(dataset)
    field_stats: Dict[str, FieldStats] = {name: FieldStats() for name in text_fields}
    seq_field_stats: Dict[str, SequenceFieldStats] = {
        name: SequenceFieldStats() for name in seq_text_fields
    }
    language_counter: Counter[str] = Counter()

    iterable: Iterable[Dict[str, object]]
    if isinstance(dataset, IterableDataset):
        iterable = dataset.take(max_samples) if max_samples else dataset
    else:
        iterable = dataset if max_samples is None else dataset.select(range(min(max_samples, len(dataset))))

    for idx, example in enumerate(iterable):
        if max_samples is not None and idx >= max_samples:
            break

        for field in text_fields:
            value = example.get(field)
            if isinstance(value, str) and value:
                field_stats[field].char_lengths.append(len(value))
                if tokenizer is not None:
                    tokens = tokenizer(
                        value,
                        add_special_tokens=False,
                        truncation=False,
                        return_attention_mask=False,
                        return_token_type_ids=False,
                    )["input_ids"]
                    field_stats[field].token_lengths.append(len(tokens))

        for field in seq_text_fields:
            value = example.get(field)
            if isinstance(value, (list, tuple)) and value:
                seq_field_stats[field].step_lengths.append(len(value))
                texts = [item for item in value if isinstance(item, str) and item]
                for text in texts:
                    seq_field_stats[field].item_char_lengths.append(len(text))
                if tokenizer is not None:
                    encoded = tokenizer(
                        texts,
                        add_special_tokens=False,
                        truncation=False,
                        return_attention_mask=False,
                        return_token_type_ids=False,
                    )
                    for ids in encoded["input_ids"]:
                        seq_field_stats[field].item_token_lengths.append(len(ids))

        for field in language_fields:
            lang_value = example.get(field)
            if isinstance(lang_value, str) and lang_value:
                language_counter[lang_value.strip()] += 1

    dataset_size = len(dataset) if not isinstance(dataset, IterableDataset) else math.nan

    summary = {
        "dataset_name": dataset_name,
        "num_samples": dataset_size,
        "sampled": max_samples if max_samples is not None else "all",
        "text_fields": {
            name: stats.to_summary() for name, stats in field_stats.items() if stats.char_lengths
        },
        "sequence_fields": {
            name: stats.to_summary() for name, stats in seq_field_stats.items() if stats.step_lengths
        },
        "languages": language_counter.most_common() if language_counter else [],
    }
    return summary


def main() -> None:
    parser = argparse.ArgumentParser(description="Analyze HF datasets used by PRM benchmarks.")
    parser.add_argument(
        "--tokenizer",
        type=str,
        default=None,
        help="Tokenizer name or path used to compute token counts (default: skip token stats).",
    )
    parser.add_argument(
        "--max-samples",
        type=int,
        default=None,
        help="Optional cap on samples per dataset to speed up analysis.",
    )
    parser.add_argument(
        "--output",
        type=str,
        default=None,
        help="Optional path to store the JSON summary.",
    )
    parser.add_argument(
        "--pretty",
        action="store_true",
        help="Pretty-print JSON output.",
    )
    args = parser.parse_args()

    tokenizer = load_tokenizer(args.tokenizer) if args.tokenizer else None

    all_summaries: List[Dict[str, object]] = []
    for name, (path, split) in DATASETS.items():
        print(f"\n[INFO] Loading dataset {name}: {path} ({split})")
        dataset = load_dataset(path, split=split)
        summary = gather_stats(name, dataset, tokenizer, args.max_samples)
        all_summaries.append(summary)
        print_dataset_summary(summary)

    if args.output:
        print(f"\n[INFO] Saving summary to {args.output}")
        with open(args.output, "w", encoding="utf-8") as f:
            json.dump(all_summaries, f, indent=2 if args.pretty else None)


def print_dataset_summary(summary: Dict[str, object]) -> None:
    dataset_name = summary["dataset_name"]
    num_samples = summary["num_samples"]
    sampled = summary["sampled"]
    text_fields = summary["text_fields"]
    sequence_fields = summary["sequence_fields"]
    languages = summary["languages"]

    print(f"=== {dataset_name} ===")
    if isinstance(num_samples, (int, float)) and not math.isnan(num_samples):
        print(f"Total samples      : {int(num_samples)}")
    else:
        print("Total samples      : streaming/unknown")
    print(f"Samples analyzed   : {sampled}")

    if text_fields:
        print("Text fields:")
        for field, stats in text_fields.items():
            char_stats = stats.get("char_lengths", {})
            token_stats = stats.get("token_lengths", {})
            print(f"  - {field}")
            if char_stats:
                print(f"      chars  -> mean: {char_stats['mean']:.1f}, median: {char_stats['median']:.1f}, "
                      f"p95: {char_stats['p95']:.1f}, max: {char_stats['max']}")
            if token_stats:
                print(f"      tokens -> mean: {token_stats['mean']:.1f}, median: {token_stats['median']:.1f}, "
                      f"p95: {token_stats['p95']:.1f}, max: {token_stats['max']}")

    if sequence_fields:
        print("Sequence fields:")
        for field, stats in sequence_fields.items():
            steps = stats.get("steps", {})
            item_chars = stats.get("item_char_lengths", {})
            item_tokens = stats.get("item_token_lengths", {})
            print(f"  - {field}")
            if steps:
                print(f"      steps  -> mean: {steps['mean']:.2f}, median: {steps['median']:.2f}, "
                      f"p95: {steps['p95']:.2f}, max: {steps['max']}")
            if item_chars:
                print(f"      item chars  -> mean: {item_chars['mean']:.1f}, median: {item_chars['median']:.1f}, "
                      f"p95: {item_chars['p95']:.1f}, max: {item_chars['max']}")
            if item_tokens:
                print(f"      item tokens -> mean: {item_tokens['mean']:.1f}, median: {item_tokens['median']:.1f}, "
                      f"p95: {item_tokens['p95']:.1f}, max: {item_tokens['max']}")

    if languages:
        print("Languages (top 10):")
        for lang, count in languages[:10]:
            print(f"  - {lang}: {count}")
    else:
        print("Languages         : no explicit language column detected")


if __name__ == "__main__":
    main()
