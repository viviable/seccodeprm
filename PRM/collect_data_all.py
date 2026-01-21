### old version, for local dataset
import json
from pathlib import Path
from typing import Iterable

from datasets import Dataset, DatasetDict, Features, Value, concatenate_datasets, load_from_disk, load_dataset
from datasets.features.features import Sequence, register_feature


# Older exports stored lists with "_type": "List" (or sometimes "list").
for legacy_name in ("List", "list"):
    register_feature(Sequence, legacy_name)


def load_dataset_dict(path: str) -> DatasetDict:
    """Load a dataset from disk and normalize it to a DatasetDict."""
    try:
        # dataset = load_from_disk(path)
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
        # Some datasets were saved as a single split without a dataset_dict.json
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
    shard_paths = [
        split_path / shard_info["filename"] for shard_info in state.get("_data_files", [])
    ]
    if not shard_paths:
        raise FileNotFoundError(f"No data shards listed in {state_path}")

    shards = [Dataset.from_file(str(shard_path)) for shard_path in shard_paths]
    return shards[0] if len(shards) == 1 else concatenate_datasets(shards)


def _canonical_split_name(name: str) -> str:
    """Map different validation spellings to the canonical 'validation' split."""
    lower_name = name.lower()
    if lower_name in {"val", "valid", "validation"}:
        return "validation"
    return name


def concatenate_splits(datasets: Iterable[DatasetDict]) -> DatasetDict:
    """Concatenate datasets split-wise (train/train, test/test, etc.)."""
    combined = DatasetDict()
    for dataset in datasets:
        for split_name, split_ds in dataset.items():
            canonical_name = _canonical_split_name(split_name)
            if canonical_name in combined:
                combined[canonical_name] = concatenate_datasets([combined[canonical_name], split_ds])
            else:
                combined[canonical_name] = split_ds
    return combined


def concat_hf_datasets(
    source_paths: Iterable[str],
    repli_num: Iterable[int],
) -> DatasetDict:
    """Concatenate HF Hub datasets by split, repeating each dataset as needed."""
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


def _vul_code_sven_features() -> Features:
    return Features(
        {
            "prompt": Value("string"),
            "completions": Sequence(Value("string")),
            "labels": Sequence(Value("int64")),
            "source": Value("string"),
            "other_info": {
                "char_changes": {
                    "added": Sequence(
                        {"char_end": Value("int64"), "char_start": Value("int64"), "chars": Value("string")}
                    ),
                    "deleted": Sequence(
                        {"char_end": Value("int64"), "char_start": Value("int64"), "chars": Value("string")}
                    ),
                },
                "commit_link": Value("string"),
                "file_name": Value("string"),
                "func_name": Value("string"),
                "line_changes": {
                    "added": Sequence(
                        {
                            "char_end": Value("int64"),
                            "char_start": Value("int64"),
                            "line": Value("string"),
                            "line_no": Value("int64"),
                        }
                    ),
                    "deleted": Sequence(
                        {
                            "char_end": Value("int64"),
                            "char_start": Value("int64"),
                            "line": Value("string"),
                            "line_no": Value("int64"),
                        }
                    ),
                },
                "vul_type": Value("string"),
            },
            "index": Value("int64"),
        }
    )


def concat_hf_datasets_local(
    source_paths: Iterable[str],
    repli_num: Iterable[int],
) -> DatasetDict:
    """Concatenate HF Hub datasets, fixing known schema issues locally."""
    datasets = []
    for path, num in zip(source_paths, repli_num):
        for _ in range(num):
            # if path == "vivi-yu/vul_code_sven":
            #     ds = load_dataset(path, features=_vul_code_sven_features())
            # else:
            ds = load_dataset(path)
            if isinstance(ds, DatasetDict):
                datasets.append(ds)
            else:
                datasets.append(DatasetDict({"train": ds}))
    return concatenate_splits(datasets)


def main() -> None:
    source_paths = [
        "vivi-yu/reposvul_processed_dataset_cleaned",
        # "/project/flame/wyu3/PRM/bigvul_processed_dataset",
        "vivi-yu/vul_code_precise",
        "vivi-yu/primevul_processed_dataset",
        "vivi-yu/vul_code_sven",
    ]
    
    # repli_num = [5, 1, 30, 600]
    repli_num = [3, 1, 3, 40]
    # repli_num = [1]
    # repli_num = [3, 1, 10, 40]
    
    
    
    datasets = []
    for path, num in zip(source_paths, repli_num):
        for _ in range(num):
            datasets.append(load_dataset_dict(path))
            # datasets.append(load_dataset("vivi-yu/reposvul_processed_dataset"))
    # merged = concatenate_splits(datasets)
    merged = concat_hf_datasets(source_paths, repli_num)
    output_dir = Path("/project/flame/wyu3/PRM/all_processed_dataset_31340")
    merged.save_to_disk(output_dir.as_posix())




if __name__ == "__main__":
    main()
