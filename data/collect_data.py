"""Unified data collection entrypoint.

This script merges the legacy collect_data_*.py scripts into one CLI-driven
module. Each subcommand mirrors the previous per-dataset script behavior.
"""

import argparse
import ast
import json
import os
import re
from difflib import SequenceMatcher
from pathlib import Path
from typing import Iterable, List, Tuple

from datasets import Dataset, DatasetDict, Features, Value, concatenate_datasets, load_dataset, load_from_disk
from datasets.features.features import Sequence, register_feature
from tqdm import tqdm

# Older exports stored lists with "_type": "List" (or sometimes "list").
for legacy_name in ("List", "list"):
    register_feature(Sequence, legacy_name)

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
    """Compare two code blocks and return segmented parts with labels."""

    def normalize_line(line: str) -> str:
        return line.strip()

    lines1 = code1.strip().split("\n")
    lines2 = code2.strip().split("\n")
    matcher = SequenceMatcher(None, lines1, lines2)
    matching_blocks = matcher.get_matching_blocks()

    list1 = []
    list2 = []
    i1, i2 = 0, 0

    for match in matching_blocks:
        a, b, size = match.a, match.b, match.size
        if i1 < a:
            diff_lines1 = lines1[i1:a]
            list1.append(
                {
                    "content": "\n".join(diff_lines1),
                    "type": "diff",
                    "lines": f"{i1+1}-{a}" if i1 + 1 != a else str(i1 + 1),
                }
            )
        if i2 < b:
            diff_lines2 = lines2[i2:b]
            list2.append(
                {
                    "content": "\n".join(diff_lines2),
                    "type": "diff",
                    "lines": f"{i2+1}-{b}" if i2 + 1 != b else str(i2 + 1),
                }
            )
        if (i1 < a) != (i2 < b):
            if i1 >= a:
                list1.append({"content": "", "type": "diff", "lines": f"after-{i1}"})
            if i2 >= b:
                list2.append({"content": "", "type": "diff", "lines": f"after-{i2}"})
        if size > 0:
            same_lines1 = lines1[a : a + size]
            same_lines2 = lines2[b : b + size]
            list1.append(
                {
                    "content": "\n".join(same_lines1),
                    "type": "same",
                    "lines": f"{a+1}-{a+size}" if size > 1 else str(a + 1),
                }
            )
            list2.append(
                {
                    "content": "\n".join(same_lines2),
                    "type": "same",
                    "lines": f"{b+1}-{b+size}" if size > 1 else str(b + 1),
                }
            )
        i1, i2 = a + size, b + size

    return list1, list2


def load_dataset_dict(path: str) -> DatasetDict:
    """Load a dataset from disk and normalize it to a DatasetDict."""
    try:
        dataset = load_dataset(path)
    except ValueError as err:
        if "Type mismatch" not in str(err):
            raise
        dataset = _load_dataset_dict_from_arrow(path)

    if isinstance(dataset, DatasetDict):
        return dataset
    return DatasetDict({"train": dataset})


def _load_dataset_dict_from_arrow(path: str) -> DatasetDict:
    """Fallback loader for legacy exports whose metadata no longer matches."""
    path_obj = Path(path)
    dataset_dict_path = path_obj / "dataset_dict.json"
    if dataset_dict_path.exists():
        splits = json.loads(dataset_dict_path.read_text()).get("splits", [])
    else:
        splits = [""]

    dataset = DatasetDict()
    if not splits or splits == [""]:
        dataset["train"] = _load_split_from_arrow(path_obj)
        return dataset

    for split in splits:
        split_path = path_obj / split
        if not split_path.exists():
            continue
        dataset[split] = _load_split_from_arrow(split_path)

    if not dataset:
        raise FileNotFoundError(f"No usable splits found under {path}")

    return dataset


def _load_split_from_arrow(split_path: Path) -> Dataset:
    """Load a single split by concatenating its Arrow shards."""
    state_path = split_path / "state.json"
    if not state_path.exists():
        raise FileNotFoundError(f"Missing state.json under {split_path}")

    state = json.loads(state_path.read_text())
    shard_paths = [split_path / shard_info["filename"] for shard_info in state.get("_data_files", [])]
    if not shard_paths:
        raise FileNotFoundError(f"No data shards listed in {state_path}")

    shards = [Dataset.from_file(str(shard_path)) for shard_path in shard_paths]
    return shards[0] if len(shards) == 1 else concatenate_datasets(shards)


def _canonical_split_name(name: str) -> str:
    lower_name = name.lower()
    if lower_name in {"val", "valid", "validation"}:
        return "validation"
    return name


def concatenate_splits(datasets: Iterable[DatasetDict]) -> DatasetDict:
    combined = DatasetDict()
    for dataset in datasets:
        for split_name, split_ds in dataset.items():
            canonical_name = _canonical_split_name(split_name)
            if canonical_name in combined:
                combined[canonical_name] = concatenate_datasets([combined[canonical_name], split_ds])
            else:
                combined[canonical_name] = split_ds
    return combined


def concat_hf_datasets(source_paths: Iterable[str], repli_num: Iterable[int]) -> DatasetDict:
    datasets = []
    for path, num in zip(source_paths, repli_num):
        for _ in range(num):
            ds = load_dataset(path)
            if isinstance(ds, DatasetDict):
                for split_name, split_ds in ds.items():
                    if "other_info" in split_ds.column_names:
                        ds[split_name] = split_ds.remove_columns("other_info")
                datasets.append(ds)
            else:
                if "other_info" in ds.column_names:
                    ds = ds.remove_columns("other_info")
                datasets.append(DatasetDict({"train": ds}))
    return concatenate_splits(datasets)


def concat_hf_datasets_local(source_paths: Iterable[str], repli_num: Iterable[int]) -> DatasetDict:
    datasets = []
    for path, num in zip(source_paths, repli_num):
        for _ in range(num):
            ds = load_dataset(path)
            if isinstance(ds, DatasetDict):
                datasets.append(ds)
            else:
                datasets.append(DatasetDict({"train": ds}))
    return concatenate_splits(datasets)


def one_zero_dataset(source_path: str, output_path: str) -> None:
    dataset = load_from_disk(source_path)
    new_dataset = DatasetDict()

    for split_name in ["test", "train", "validation"]:
        if split_name not in dataset:
            continue
        new_dataset[split_name] = []
        for data in dataset[split_name]:
            if 0 in data["labels"]:
                first_zero_index = data["labels"].index(0)
                new_labels = data["labels"].copy()
                for i in range(first_zero_index, len(new_labels)):
                    new_labels[i] = 0
                new_data = {
                    "labels": new_labels,
                    "completions": data["completions"],
                    "prompt": data["prompt"],
                    "source": data["source"],
                    "other_info": data.get("other_info"),
                    "index": data["index"],
                }
                new_dataset[split_name].append(new_data)
            else:
                new_dataset[split_name].append(data)
        new_dataset[split_name] = Dataset.from_list(new_dataset[split_name])
    new_dataset.save_to_disk(output_path)


# -------------------------
# BigVul
# -------------------------

def bigvul_main(path: str) -> None:
    vul_dataset = DatasetDict()
    splits = ["test", "train", "validation"]
    for split_name in splits:
        print(f"Processing {split_name} split...")
        dataset = load_dataset("bstee615/bigvul")[split_name]
        split_data = []
        for index, data in enumerate(tqdm(dataset, desc=f"Processing {split_name}")):
            if data["vul"] == 0:
                code = data["func_after"]
                completions = code.split("\n\n")
                prompt = "Determine whether the code is vulnerable or not."
                labels = [1] * len(completions)
                source = "BIGVUL"
                other_info = {k: data[k] for k in data.keys() if k not in ["func_before", "func_after", "vul"]}
                split_data.append(
                    {
                        "prompt": prompt,
                        "completions": completions,
                        "labels": labels,
                        "source": source,
                        "vul": data,
                        "other_info": other_info,
                        "index": index,
                    }
                )
            elif data["vul"] == 1:
                code1 = data["func_before"]
                code2 = data["func_after"]
                code1_blocks, code2_blocks = compare_code_blocks(code1, code2)
                prompt = "Determine whether the code is vulnerable or not."

                completion1 = []
                labels1 = []
                for block in code1_blocks:
                    completion1.extend(block["content"].split("\n\n"))
                    labels1.extend([0 if block["type"] == "diff" else 1] * len(block["content"].split("\n\n")))

                completion2 = []
                labels2 = []
                for block in code2_blocks:
                    completion2.extend(block["content"].split("\n\n"))
                    labels2.extend([1] * len(block["content"].split("\n\n")))

                source = "BIGVUL"
                other_info = {k: data[k] for k in data.keys() if k not in ["func_before", "func_after", "vul"]}

                split_data.append(
                    {
                        "prompt": prompt,
                        "completions": completion1,
                        "labels": labels1,
                        "source": source,
                        "other_info": other_info,
                        "index": index,
                    }
                )
                split_data.append(
                    {
                        "prompt": prompt,
                        "completions": completion2,
                        "labels": labels2,
                        "source": source,
                        "other_info": other_info,
                        "index": index,
                    }
                )

        vul_dataset[split_name] = Dataset.from_list(split_data)
        print(f"Completed {split_name} split: {len(split_data)} examples")

    vul_dataset.save_to_disk(path)


# -------------------------
# PreciseBugs
# -------------------------


def precise_load_data(dir_path: str) -> Dataset:
    data = []
    for file_name in os.listdir(dir_path):
        with open(os.path.join(dir_path, file_name, "BugEntry.json"), "r") as f:
            data.append(json.loads(f.read()))
    return Dataset.from_list(data)


def precise_main(path: str, dir_path: str, cwe_filter: str = "CWE-639") -> None:
    vul_dataset = DatasetDict()
    dataset = precise_load_data(dir_path)
    split_data = []

    for index, data in enumerate(tqdm(dataset, desc="Processing train")):
        if data["type"] != cwe_filter:
            continue
        for i in range(len(data["buggy_code"])):
            code1 = data["buggy_code"][i]
            code2 = data["fixing_code"][i]
            code1_blocks, code2_blocks = compare_code_blocks(code1, code2)
            prompt = "Determine whether the {function_name} code is vulnerable or not."

            completion1 = []
            labels1 = []
            for block in code1_blocks:
                completion1.extend(block["content"].split("\n\n"))
                labels1.extend([0 if block["type"] == "diff" else 1] * len(block["content"].split("\n\n")))
            if 0 not in labels1 and labels1:
                labels1[-1] = 0

            completion2 = []
            labels2 = []
            for block in code2_blocks:
                completion2.extend(block["content"].split("\n\n"))
                labels2.extend([1] * len(block["content"].split("\n\n")))

            source = "PreciseBugs"
            other_info = {k: data[k] for k in data.keys() if k not in ["buggy_code", "fixing_code"]}
            filename = data["filenames"][0] if len(data["filenames"]) == 1 else data["filenames"][-1]
            try:
                language = filename.split(".")[1]
            except Exception:
                language = "None"

            split_data.append(
                {
                    "prompt": prompt,
                    "completions": completion1,
                    "labels": labels1,
                    "source": source,
                    "other_info": other_info,
                    "cwe": data["type"].lower(),
                    "index": index,
                    "language": language,
                }
            )
            split_data.append(
                {
                    "prompt": prompt,
                    "completions": completion2,
                    "labels": labels2,
                    "source": source,
                    "other_info": other_info,
                    "index": index,
                    "language": language,
                    "cwe": data["type"].lower(),
                }
            )

    total_number = len(split_data)
    train_data = split_data[: int(total_number * 0.8)]
    val_data = split_data[int(total_number * 0.8) : int(total_number * 0.9)]
    test_data = split_data[int(total_number * 0.9) :]

    vul_dataset["train"] = Dataset.from_list(train_data)
    vul_dataset["val"] = Dataset.from_list(val_data)
    vul_dataset["test"] = Dataset.from_list(test_data)

    vul_dataset.save_to_disk(path)


# -------------------------
# PrimeVul
# -------------------------

def primevul_load_data(split_name: str, paired: bool, base_dir: str) -> Dataset:
    if paired:
        json_path = os.path.join(base_dir, f"primevul_{split_name}_paired.jsonl")
    else:
        json_path = os.path.join(base_dir, f"primevul_{split_name}.jsonl")

    data = []
    with open(json_path, "r") as f:
        for line in f:
            data.append(json.loads(line))
    data = primevul_preprocess_data(data)
    return Dataset.from_list(data)


def primevul_preprocess_data(data):
    for item in data:
        if "func_hash" in item:
            item["func_hash"] = str(item["func_hash"])
        if "file_hash" in item:
            item["file_hash"] = str(item["file_hash"])
    return data


def primevul_main(path: str, base_dir: str, paired: bool) -> None:
    vul_dataset = DatasetDict()
    splits = ["test", "train", "valid"]

    for split_name in splits:
        dataset = primevul_load_data(split_name, paired=paired, base_dir=base_dir)
        split_data = []
        for index, data in enumerate(tqdm(dataset, desc=f"Processing {split_name}")):
            if paired:
                data_pair = None
                for data_ in dataset:
                    if data_["commit_id"] == data["commit_id"] and data_["idx"] != data["idx"]:
                        data_pair = data_
                        break
                if data_pair is None:
                    print(f"no data pair: {data['idx']}")
                    continue
                if data["target"] == 0 and data_pair["target"] == 1:
                    safe_data = data
                    vul_data = data_pair
                elif data["target"] == 1 and data_pair["target"] == 0:
                    vul_data = data
                    safe_data = data_pair
                else:
                    print(f"Invalid data: {data['target']} and {data_pair['target']}")
                    continue

                code1_blocks, code2_blocks = compare_code_blocks(vul_data["func"], safe_data["func"])
                prompt = "Determine whether the {function_name} code is vulnerable or not."

                completion1 = []
                labels1 = []
                for block in code1_blocks:
                    completion1.extend(block["content"].split("\n\n"))
                    labels1.extend([0 if block["type"] == "diff" else 1] * len(block["content"].split("\n\n")))
                if 0 not in labels1 and labels1:
                    labels1[-1] = 0

                completion2 = []
                labels2 = []
                for block in code2_blocks:
                    completion2.extend(block["content"].split("\n\n"))
                    labels2.extend([1] * len(block["content"].split("\n\n")))

                source = "PrimeVul"
                other_info_vul = {k: vul_data[k] for k in vul_data.keys() if k not in ["target", "cwe", "file_name"]}
                other_info_safe = {k: safe_data[k] for k in safe_data.keys() if k not in ["target", "cwe", "file_name"]}

                cwe = vul_data["cwe"][0].lower()
                try:
                    language = vul_data["file_name"].split(".")[1]
                except Exception:
                    language = "unknown"

                split_data.append(
                    {
                        "prompt": prompt.format(function_name=data["project"]),
                        "completions": completion1,
                        "labels": labels1,
                        "source": source,
                        "cwe": cwe,
                        "language": language,
                        "other_info": other_info_vul,
                        "index": index,
                    }
                )
                split_data.append(
                    {
                        "prompt": prompt.format(function_name=data["project"]),
                        "completions": completion2,
                        "labels": labels2,
                        "source": source,
                        "cwe": cwe,
                        "language": language,
                        "other_info": other_info_safe,
                        "index": index,
                    }
                )
            else:
                prompt = "Determine whether the {function_name} code is vulnerable or not."
                source = "PrimeVul_unpaired"
                completion = data["func"].split("\n\n")
                labels = [1] * len(completion) if data["target"] == 0 else [0] * len(completion)
                other_info = {k: data[k] for k in data.keys() if k not in ["func", "target"]}
                split_data.append(
                    {
                        "prompt": prompt.format(function_name=data["project"]),
                        "completions": completion,
                        "labels": labels,
                        "source": source,
                        "other_info": other_info,
                        "index": index,
                    }
                )

        vul_dataset[split_name] = Dataset.from_list(split_data)
        print(f"Completed {split_name} split: {len(split_data)} examples")

    vul_dataset.save_to_disk(path)


# -------------------------
# ReposVul
# -------------------------

def reposvul_preprocess_data(data):
    for item in data:
        for key, value in item.items():
            if value == [] or (isinstance(value, list) and len(value) == 0):
                print(f"Empty list found in column: {key}")
                item[key] = None
            if key == "detail_info" and isinstance(value, dict):
                for k, v in value.items():
                    if v == [] or (isinstance(v, list) and len(v) == 0):
                        print(f"Empty list found in column: {k}")
                        item["detail_info"][k] = None
    return data


def reposvul_load_data(language_name: str, base_dir: str) -> Dataset:
    json_path = os.path.join(base_dir, f"ReposVul_{language_name}.jsonl")
    data = []
    with open(json_path, "r", encoding="utf-8") as f:
        for line in f:
            single_data = json.loads(line)
            single_data["detail_info"] = {}
            for i, detail in enumerate(single_data["details"]):
                if "target" in detail:
                    single_data["detail_info"][f"{i}"] = detail
            if single_data["detail_info"] == {}:
                continue
            single_data.pop("details")
            data.append(single_data)

    dataset = reposvul_preprocess_data(data)
    for item in dataset:
        if "detail_info" in item:
            item["detail_info"] = json.dumps(item["detail_info"])

    return Dataset.from_list(dataset)


def reposvul_main(path: str, base_dir: str, languages: List[str]) -> None:
    vul_dataset = DatasetDict()
    train_data = []
    test_data = []
    prompt = "Determine whether the code is vulnerable or not."
    source = "REPOSVUL"

    for language in languages:
        print(f"Processing {language} split...")
        dataset = reposvul_load_data(language, base_dir=base_dir)
        split_data = []
        for index, data in enumerate(tqdm(dataset, desc=f"Processing {language}")):
            detail_info = json.loads(data["detail_info"])
            for i, i_detail in detail_info.items():
                outdated = i_detail.get("outdated")
                if outdated:
                    continue
                try:
                    if "function_before" in i_detail:
                        for function_index, single_function in enumerate(i_detail["function_before"]):
                            if single_function["target"] == 0:
                                completions = single_function["function"].split("\n\n")
                                labels = [1] * len(completions)
                                split_data.append(
                                    {
                                        "prompt": prompt,
                                        "completions": completions,
                                        "labels": labels,
                                        "source": source,
                                        "index": index,
                                        "language": language,
                                        "cwe": data["cwe_id"],
                                    }
                                )
                            elif single_function["target"] == 1 and i_detail["function_after"][function_index]["target"] == 0:
                                code1_blocks, code2_blocks = compare_code_blocks(
                                    single_function["function"],
                                    i_detail["function_after"][function_index]["function"],
                                )
                                completion1 = []
                                labels1 = []
                                for block in code1_blocks:
                                    completion1.extend(block["content"].split("\n\n"))
                                    labels1.extend([0 if block["type"] == "diff" else 1] * len(block["content"].split("\n\n")))

                                completion2 = []
                                labels2 = []
                                for block in code2_blocks:
                                    completion2.extend(block["content"].split("\n\n"))
                                    labels2.extend([1] * len(block["content"].split("\n\n")))

                                split_data.append(
                                    {
                                        "prompt": prompt,
                                        "completions": completion1,
                                        "labels": labels1,
                                        "source": source,
                                        "index": index,
                                        "language": language,
                                        "cwe": data["cwe_id"],
                                    }
                                )
                                split_data.append(
                                    {
                                        "prompt": prompt,
                                        "completions": completion2,
                                        "labels": labels2,
                                        "source": source,
                                        "index": index,
                                        "language": language,
                                        "cwe": data["cwe_id"],
                                    }
                                )
                except Exception:
                    print(f"Error processing {index} {language}")
        number = len(split_data)
        train_data.extend(split_data[: int(number * 0.8)])
        test_data.extend(split_data[int(number * 0.8) :])

    vul_dataset["train"] = Dataset.from_list(train_data)
    vul_dataset["test"] = Dataset.from_list(test_data)
    print(f"Completed {language} split: {len(train_data)} examples")
    print(f"Completed {language} test split: {len(test_data)} examples")
    vul_dataset.save_to_disk(path)


# -------------------------
# ReposVul relabel (AST/clang)
# -------------------------

def _build_step_line_ranges(completions):
    line_ranges = []
    current_start = 1
    for completion in completions:
        if not completion:
            line_ranges.append((current_start, current_start - 1))
            continue
        line_count = completion.count("\n") + 1
        line_ranges.append((current_start, current_start + line_count - 1))
        current_start += line_count + 2
    return line_ranges


def _line_to_step(line_ranges, lineno):
    for idx, (start, end) in enumerate(line_ranges):
        if start <= lineno <= end:
            return idx
    return None


def _c_has_function_definition(block, func_name: str) -> bool:
    if not func_name:
        return False
    pattern = r"^\s*(?:[\w:\<\>\~\*&]+\s+)+" + re.escape(func_name) + r"\s*\([^;]*\)\s*\{"
    return re.search(pattern, block, re.MULTILINE) is not None


def _c_has_function_call(block, func_name: str) -> bool:
    if not func_name:
        return False
    call_pattern = r"\b" + re.escape(func_name) + r"\s*\("
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
        if re.search(r"\b[A-Za-z_]\w*\s*\(", block):
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
        results.append({"step": step, "function_name": function_name, "completion": block})
    return results


def extract_function_name_from_block(block):
    python_def = re.search(r"^\s*def\s+([A-Za-z_]\w*)\s*\(", block, re.MULTILINE)
    if python_def:
        return python_def.group(1)

    c_like_def = re.search(
        r"^\s*(?:[\w:\<\>\~\*&]+\s+)+([A-Za-z_]\w*)\s*\([^;]*\)\s*\{",
        block,
        re.MULTILINE,
    )
    if c_like_def:
        return c_like_def.group(1)

    return None


def bump_labels_for_used_steps(completions, labels, bump=0.5):
    PARSE_STATS["samples"] += 1
    zero_label_blocks = []
    for i, (completion, label) in enumerate(zip(completions, labels)):
        if label == 0 and completion != "":
            zero_label_blocks.append((i, completion))

    if not zero_label_blocks:
        return labels
    PARSE_STATS["samples_with_zero_label_blocks"] += 1

    results = find_step_function_names(zero_label_blocks, completions)
    updated_labels = labels[:]
    used_steps = set()
    for result in results:
        steps_calling_function = find_steps_calling_function(completions, result["function_name"])
        steps_called_by_function = find_steps_called_by_function(completions, result["function_name"])
        used_steps.update(steps_calling_function)
        used_steps.update(steps_called_by_function)

    for step in used_steps:
        if 0 <= step < len(updated_labels):
            updated_labels[step] -= bump

    return updated_labels


def relabel_dataset(dataset_train):
    def _update(example):
        example["labels"] = bump_labels_for_used_steps(example["completions"], example["labels"])
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


def reposvul_relabel_main(source_path: str, output_path: str) -> None:
    old_dataset = load_dataset(source_path)
    new_dataset = DatasetDict({"train": relabel_dataset(old_dataset["train"]), "test": old_dataset["test"]})
    new_dataset.save_to_disk(output_path)


# -------------------------
# Sven
# -------------------------

def sven_load_data(split_name: str, base_dir: str, vul_list: List[int]) -> Dataset:
    data = []
    for vul in vul_list:
        file_name = f"cwe-{vul:03d}.jsonl"
        with open(os.path.join(base_dir, split_name, file_name), "r") as f:
            data.extend([json.loads(line) for line in f])
    return Dataset.from_list(data)


def sven_main(path: str, base_dir: str, vul_list: List[int]) -> None:
    vul_dataset = DatasetDict()
    splits = ["train", "val"]
    for split_name in splits:
        print(f"Processing {split_name} split...")
        dataset = sven_load_data(split_name, base_dir=base_dir, vul_list=vul_list)
        split_data = []
        for index, data in enumerate(tqdm(dataset, desc=f"Processing {split_name}")):
            code1 = data["func_src_before"]
            code2 = data["func_src_after"]
            code1_blocks, code2_blocks = compare_code_blocks(code1, code2)
            prompt = "Determine whether the {function_name} code is vulnerable or not."

            completion1 = []
            labels1 = []
            for block in code1_blocks:
                completion1.extend(block["content"].split("\n\n"))
                labels1.extend([0 if block["type"] == "diff" else 1] * len(block["content"].split("\n\n")))
            if 0 not in labels1 and labels1:
                labels1[-1] = 0

            completion2 = []
            labels2 = []
            for block in code2_blocks:
                completion2.extend(block["content"].split("\n\n"))
                labels2.extend([1] * len(block["content"].split("\n\n")))

            source = "SVEN"
            other_info = {k: data[k] for k in data.keys() if k not in ["func_src_before", "func_src_after"]}

            split_data.append(
                {
                    "prompt": prompt,
                    "completions": completion1,
                    "labels": labels1,
                    "source": source,
                    "other_info": other_info,
                    "index": index,
                    "cwe": data["vul_type"],
                    "language": data["file_name"].split(".")[1],
                }
            )
            split_data.append(
                {
                    "prompt": prompt,
                    "completions": completion2,
                    "labels": labels2,
                    "source": source,
                    "other_info": other_info,
                    "index": index,
                    "cwe": data["vul_type"],
                    "language": data["file_name"].split(".")[1],
                }
            )

        vul_dataset[split_name] = Dataset.from_list(split_data)
        print(f"Completed {split_name} split: {len(split_data)} examples")

    vul_dataset.save_to_disk(path)


# -------------------------
# All datasets concat
# -------------------------


def concat_all_main(output_dir: str, source_paths: List[str], repli_num: List[int]) -> None:
    datasets = []
    for path, num in zip(source_paths, repli_num):
        for _ in range(num):
            datasets.append(load_dataset_dict(path))
    merged = concat_hf_datasets(source_paths, repli_num)
    Path(output_dir).mkdir(parents=True, exist_ok=True)
    merged.save_to_disk(output_dir)


# -------------------------
# CLI
# -------------------------


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Unified data collection for SecCodePRM")
    subparsers = parser.add_subparsers(dest="command", required=True)

    bigvul = subparsers.add_parser("bigvul", help="Process BigVul dataset")
    bigvul.add_argument("--output", default="./bigvul_processed_dataset")

    precise = subparsers.add_parser("precise", help="Process PreciseBugs dataset")
    precise.add_argument("--output", default="./precisebugs_processed_dataset")
    precise.add_argument(
        "--dir",
        dest="dir_path",
        default="/project/flame/wyu3/PRM/PreciseBugs/CVEs",
        help="Path to PreciseBugs CVEs directory",
    )
    precise.add_argument("--cwe", dest="cwe", default="CWE-639")

    primevul = subparsers.add_parser("primevul", help="Process PrimeVul dataset")
    primevul.add_argument("--output", default="/project/flame/wyu3/PRM/primevul_processed_dataset_paired")
    primevul.add_argument("--base-dir", default="/project/flame/wyu3/PRM/PrimeVul_v0.1")
    primevul.add_argument("--paired", action="store_true")

    reposvul = subparsers.add_parser("reposvul", help="Process ReposVul dataset")
    reposvul.add_argument("--output", default="/project/flame/wyu3/PRM/reposvul_processed_dataset")
    reposvul.add_argument("--base-dir", default="/project/flame/wyu3/PRM/ReposVul")
    reposvul.add_argument("--languages", nargs="+", default=["c", "cpp", "java", "python"])

    reposvul_relabel = subparsers.add_parser("reposvul-relabel", help="Relabel ReposVul dataset")
    reposvul_relabel.add_argument("--source", default="vivi-yu/reposvul_processed_dataset")
    reposvul_relabel.add_argument("--output", default="/project/flame/wyu3/PRM/reposvul_processed_dataset_relabeled")

    sven = subparsers.add_parser("sven", help="Process SVEN dataset")
    sven.add_argument("--output", default="/project/flame/wyu3/PRM/sven_processed_dataset")
    sven.add_argument("--base-dir", default="/project/flame/wyu3/PRM/sven_git/sven/data_train_val")
    sven.add_argument("--vul-list", nargs="+", type=int, default=[22, 78, 79, 89, 125, 190, 416, 476, 787])

    concat_all = subparsers.add_parser("all", help="Concat multiple datasets")
    concat_all.add_argument("--output", default="/project/flame/wyu3/PRM/all_processed_dataset_31340")
    concat_all.add_argument(
        "--sources",
        nargs="+",
        default=[
            "vivi-yu/reposvul_processed_dataset_cleaned",
            "vivi-yu/vul_code_precise",
            "vivi-yu/primevul_processed_dataset",
            "vivi-yu/vul_code_sven",
        ],
    )
    concat_all.add_argument("--repli-num", nargs="+", type=int, default=[3, 1, 3, 40])

    one_zero = subparsers.add_parser("one-zero", help="Convert dataset to one-zero labels")
    one_zero.add_argument("--source", required=True)
    one_zero.add_argument("--output", required=True)

    return parser


def main() -> None:
    parser = build_parser()
    args = parser.parse_args()

    if args.command == "bigvul":
        bigvul_main(args.output)
    elif args.command == "precise":
        precise_main(args.output, dir_path=args.dir_path, cwe_filter=args.cwe)
    elif args.command == "primevul":
        primevul_main(args.output, base_dir=args.base_dir, paired=args.paired)
    elif args.command == "reposvul":
        reposvul_main(args.output, base_dir=args.base_dir, languages=args.languages)
    elif args.command == "reposvul-relabel":
        reposvul_relabel_main(args.source, args.output)
    elif args.command == "sven":
        sven_main(args.output, base_dir=args.base_dir, vul_list=args.vul_list)
    elif args.command == "all":
        concat_all_main(args.output, source_paths=args.sources, repli_num=args.repli_num)
    elif args.command == "one-zero":
        one_zero_dataset(args.source, args.output)
    else:
        raise ValueError(f"Unknown command: {args.command}")


if __name__ == "__main__":
    main()
