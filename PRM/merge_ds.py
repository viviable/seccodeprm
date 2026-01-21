from __future__ import annotations

from typing import Iterable, List

from datasets import Dataset, DatasetDict, concatenate_datasets, load_dataset


KEEP_COLUMNS = ["prompt", "completions", "labels", "source", "index"]


def _canonical_split_name(name: str) -> str:
    lower_name = name.lower()
    if lower_name in {"val", "valid", "validation"}:
        return "validation"
    return name


def _normalize_dataset(ds: Dataset | DatasetDict) -> DatasetDict:
    if isinstance(ds, DatasetDict):
        return ds
    return DatasetDict({"train": ds})


def _select_required_columns(ds: Dataset, keep_columns: Iterable[str]) -> Dataset:
    keep_list = list(keep_columns)
    missing = [col for col in keep_list if col not in ds.column_names]
    if missing:
        raise ValueError(f"Dataset is missing required columns: {missing}")
    return ds.select_columns(keep_list)


def concatenate_splits(datasets: Iterable[DatasetDict]) -> DatasetDict:
    combined = DatasetDict()
    for dataset in datasets:
        for split_name, split_ds in dataset.items():
            canonical_name = _canonical_split_name(split_name)
            if canonical_name in combined:
                combined[canonical_name] = concatenate_datasets(
                    [combined[canonical_name], split_ds]
                )
            else:
                combined[canonical_name] = split_ds
    return combined


def merge_hf_datasets(
    source_paths: Iterable[str],
    repli_num: Iterable[int],
    keep_columns: Iterable[str] | None = None,
) -> DatasetDict:
    keep_columns = list(keep_columns or KEEP_COLUMNS)
    datasets: List[DatasetDict] = []
    for path, num in zip(source_paths, repli_num):
        ds = _normalize_dataset(load_dataset(path))
        for split_name, split_ds in ds.items():
            ds[split_name] = _select_required_columns(split_ds, keep_columns)
        for _ in range(num):
            datasets.append(ds)
    return concatenate_splits(datasets)

if __name__ == "__main__":
    source_paths = [
        "vivi-yu/reposvul_processed_dataset_cleaned",
        "vivi-yu/vul_code_precise",
        "vivi-yu/primevul_processed_dataset",
        "vivi-yu/vul_code_sven",
    ]
    repli_num = [3, 1, 3, 40]
    keep_columns = ["prompt", "completions", "labels", "source", "index"]
    merged = merge_hf_datasets(source_paths, repli_num, keep_columns)
    merged.save_to_disk("/project/flame/wyu3/PRM/all_processed_dataset_31340")