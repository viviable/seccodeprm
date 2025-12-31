import argparse
import os
from typing import Dict, Iterable, List, Optional, Sequence

import torch
from transformers import AutoModelForTokenClassification, AutoTokenizer


def _load_state_dict(model_path: str, torch_dtype: torch.dtype = torch.float32) -> Dict[str, torch.Tensor]:
    model = AutoModelForTokenClassification.from_pretrained(
        model_path,
        trust_remote_code=True,
        torch_dtype=torch_dtype,
        device_map=None,
    )
    state = {k: v.to("cpu") for k, v in model.state_dict().items()}
    del model
    torch.cuda.empty_cache()
    return state


def _average_tensors(tensors: Sequence[torch.Tensor]) -> torch.Tensor:
    stacked = [t.float() for t in tensors]
    return torch.stack(stacked, dim=0).mean(dim=0)


def _guess_head_keys(state_dict: Dict[str, torch.Tensor], num_labels: int) -> List[str]:
    keys: List[str] = []
    for name, tensor in state_dict.items():
        if not isinstance(tensor, torch.Tensor):
            continue
        if tensor.dim() == 2 and tensor.shape[0] == num_labels:
            keys.append(name)
        elif tensor.dim() == 1 and tensor.numel() == num_labels:
            keys.append(name)
    if keys:
        return sorted(set(keys))

    prefixes = ("classifier", "score", "lm_head", "classification_head", "head")
    for name in state_dict:
        parts = name.split(".")
        if any(p in parts for p in prefixes):
            keys.append(name)
    if not keys:
        raise ValueError("Could not infer classification head parameters; pass --head-keys explicitly.")
    return sorted(set(keys))


def _merge_state_dicts(
    state_dicts: List[Dict[str, torch.Tensor]],
    keys_to_merge: Iterable[str],
    base_state: Dict[str, torch.Tensor],
) -> Dict[str, torch.Tensor]:
    keys_to_merge = list(keys_to_merge)
    for key in keys_to_merge:
        if any(key not in sd for sd in state_dicts):
            raise KeyError(f"Parameter {key} missing in at least one checkpoint.")
        base_state[key] = _average_tensors([sd[key] for sd in state_dicts])
    return base_state


def merge_models(
    model_paths: List[str],
    output_dir: str,
    mode: str = "all",
    head_keys: Optional[Iterable[str]] = None,
) -> None:
    if len(model_paths) < 2:
        raise ValueError("Need at least two models to merge.")
    os.makedirs(output_dir, exist_ok=True)

    base_model = AutoModelForTokenClassification.from_pretrained(
        model_paths[0],
        trust_remote_code=True,
        torch_dtype=torch.float32,
        device_map=None,
    )
    base_state = {k: v.to("cpu") for k, v in base_model.state_dict().items()}

    state_dicts = [base_state]
    for path in model_paths[1:]:
        state_dicts.append(_load_state_dict(path))

    if mode == "all":
        keys = base_state.keys()
    elif mode == "head":
        keys = head_keys or _guess_head_keys(base_state, base_model.config.num_labels)
    else:
        raise ValueError("mode must be 'all' or 'head'.")

    merged_state = _merge_state_dicts(state_dicts, keys, base_state)
    base_model.load_state_dict(merged_state, strict=True)
    base_model.save_pretrained(output_dir)

    tokenizer = AutoTokenizer.from_pretrained(model_paths[0], trust_remote_code=True, use_fast=False)
    tokenizer.save_pretrained(output_dir)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Average PRM checkpoints of the same architecture.")
    parser.add_argument(
        "--models",
        nargs="+",
        required=True,
        help="Paths to PRM checkpoints to merge (same architecture, same number of labels).",
    )
    parser.add_argument(
        "--output",
        required=True,
        help="Where to write the merged checkpoint.",
    )
    parser.add_argument(
        "--mode",
        choices=["all", "head"],
        default="all",
        help="Merge all parameters or only the classification head.",
    )
    parser.add_argument(
        "--head-keys",
        nargs="*",
        help="Optional explicit parameter names to merge when mode=head. If omitted, will auto-detect.",
    )
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()
    merge_models(args.models, args.output, args.mode, args.head_keys)
