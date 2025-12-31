import json
from pathlib import Path
from typing import Iterable

from datasets import Dataset, DatasetDict, concatenate_datasets, load_from_disk
from datasets.features.features import Sequence, register_feature


# Older exports stored lists with "_type": "List" (or sometimes "list").
for legacy_name in ("List", "list"):
    register_feature(Sequence, legacy_name)


def load_dataset_dict(path: str) -> DatasetDict:
    """Load a dataset from disk and normalize it to a DatasetDict."""
    try:
        dataset = load_from_disk(path)
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


def main() -> None:
    # source_paths = [
    #     "/project/flame/wyu3/PRM/reposvul_processed_dataset",
    #     # "/project/flame/wyu3/PRM/bigvul_processed_dataset",
    #     "/project/flame/wyu3/PRM/precisebugs_processed_dataset",
    #     "/project/flame/wyu3/PRM/primevul_processed_dataset",
    #     "/project/flame/wyu3/PRM/sven_processed_dataset",
    # ]
    source_paths = [
        "/project/flame/wyu3/PRM/bigvul_processed_dataset",
        
    ]
    # repli_num = [5, 1, 30, 600]
    # repli_num = [3, 1, 3, 40]
    repli_num = [1]
    # repli_num = [3, 1, 10, 40]
    
    
    
    datasets = []
    for path, num in zip(source_paths, repli_num):
        for _ in range(num):
            # datasets.append(load_dataset_dict(path))
            datasets.append(load_dataset("vivi-yu/reposvul_processed_dataset"))
    merged = concatenate_splits(datasets)
    output_dir = Path("/project/flame/wyu3/PRM/all_processed_dataset_31340_pure")
    merged.save_to_disk(output_dir.as_posix())


def clean_data(dataset: DatasetDict) -> DatasetDict:
    from datasets import load_dataset
    dataset = load_dataset('vivi-yu/reposvul_processed_dataset')
    before = ["}", "\tint i;", "\t}", "#endif", "\t}", "\treturn 0;\n}", "return 0;\n}", "\t\treturn 0;", "\treturn 0;", "\treturn true;", "*/", "\treturn;", "\treturn ret;", "\treturn 0;\n\t}","\t\t}", "return 1;","break;", "continue;",  "return NULL;", ]
    after = ["\t\telse", "#else", "out:", "int i;", ]
    remove = [ "\n"]
    for split in dataset.keys():
        for data in dataset[split]:
            completions = data['completions']
            for step in completions:
                
    return dataset

if __name__ == "__main__":
    main()
