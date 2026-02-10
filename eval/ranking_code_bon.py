"""Unified BoN ranking scripts for code benchmarks.

Replaces:
- ranking_code_bon_cweval.py
- ranking_code_bon_livecodebench.py
- ranking_code_bon_sven.py
- ranking_code_bon_sven_new.py
"""

import argparse
import csv
import json
import os
import re
from typing import Dict, List

import numpy as np
import torch
from datasets import load_from_disk
from tqdm import tqdm
from transformers import AutoModel, AutoModelForTokenClassification, AutoTokenizer

from constant import CWES_TRAINED
from sven_eval import LMCodeEvaler
from vllm import LLM, SamplingParams


def load_cweval_dataset(generated_dir: str) -> Dict:
    dist_data = {}
    generated_name = os.path.basename(os.path.dirname(generated_dir)) + "/" + os.path.basename(generated_dir)
    evals_root = generated_dir

    for gen_dir in os.listdir(generated_dir):
        if gen_dir.endswith(".json"):
            continue
        res_json_path = os.path.join(generated_dir, gen_dir, "res.json")
        if not os.path.exists(res_json_path):
            continue
        with open(res_json_path, "r") as f:
            res = json.load(f)
        for file_path, labels in res.items():
            file_path = file_path.replace("test", "task")

            norm_path = os.path.normpath(file_path)
            parts = norm_path.split(os.sep)
            file_name = parts[-1]
            language = parts[-2]
            task = parts[-3]
            gen_id = parts[-4] if len(parts) >= 4 else gen_dir
            task_id = os.path.splitext(file_name)[0]
            file_path = file_path.replace(".py", f".{language}")

            dist_data.setdefault(task, {}).setdefault(language, {}).setdefault(task_id, {})

            if os.path.isabs(file_path) and os.path.exists(file_path):
                fs_path = file_path
            else:
                candidates = [
                    os.path.join(evals_root, "/".join(file_path.split("/")[2:])),
                    os.path.join(evals_root, "/".join(file_path.split("/")[3:])),
                    os.path.join(evals_root, "/".join(file_path.split("/")[4:])),
                ]
                fs_path = next((p for p in candidates if os.path.exists(p)), None)
                if fs_path is None:
                    raise FileNotFoundError(f"Could not resolve path from res.json: {file_path}")

            with open(fs_path) as f:
                code = f.read()

            dist_data[task][language][task_id][gen_id] = {
                "code": code,
                "task": task,
                "language": language,
                "index": gen_id,
                "task_id": task_id,
                "file_path": os.path.join(generated_name, gen_id, task, language),
                "label_functional": labels["functional"],
                "label_secure": labels["secure"],
            }
    return dist_data


def load_livecodebench_data(input_path: str):
    with open(input_path, "r") as f:
        return json.load(f)


def load_sven_dataset(data_dir: str):
    testset = {}
    vul_types = CWES_TRAINED
    for vul_type in vul_types:
        testset[vul_type] = []
        vul_dir = os.path.join(data_dir, vul_type)
        new_data = {}
        for scenario in list(sorted(os.listdir(vul_dir))):
            new_data[scenario] = {}
            with open(os.path.join(vul_dir, scenario, "info.json")) as f:
                info = json.load(f)
            new_data[scenario]["info"] = info
            with open(os.path.join(vul_dir, scenario, "file_context." + info["language"])) as f:
                file_context = f.read()
            with open(os.path.join(vul_dir, scenario, "func_context." + info["language"])) as f:
                func_context = f.read()
            new_data[scenario]["file_context"] = file_context
            new_data[scenario]["func_context"] = func_context
        testset[vul_type].append(new_data)
    return testset


def build_prm(prm_name: str, device: str = "auto"):
    if "qwen" in prm_name.lower():
        prm = AutoModel.from_pretrained(
            prm_name,
            device_map=device,
            dtype=torch.bfloat16,
            trust_remote_code=True,
        ).eval()
    else:
        prm = AutoModelForTokenClassification.from_pretrained(
            prm_name,
            device_map=device,
            torch_dtype=torch.bfloat16,
            trust_remote_code=True,
        ).eval()
    prm_tokenizer = AutoTokenizer.from_pretrained(prm_name, trust_remote_code=True)
    return prm, prm_tokenizer


def get_stepwise_rewards(generated_code, step_separator, step_separator_token, question_ids, prm, prm_tokenizer):
    steps = [i.rstrip() for i in generated_code.split(step_separator)]
    input_ids = question_ids.clone()

    score_ids = []
    for step in steps:
        step_ids = prm_tokenizer(step, add_special_tokens=False, return_tensors="pt")["input_ids"]
        input_ids = torch.cat([input_ids, step_ids, step_separator_token], dim=-1)
        score_ids.append(input_ids.size(-1) - 1)

    input_ids = input_ids.to(prm.device, dtype=torch.long)
    token_masks = torch.zeros_like(input_ids, dtype=torch.bool)
    token_masks[0, score_ids] = True
    assert torch.all(input_ids[token_masks].to("cpu") == step_separator_token)

    with torch.no_grad():
        logits = prm(input_ids).logits
        step_rewards = []
        for sample, token_mask in zip(logits, token_masks):
            probs = sample[token_mask].softmax(dim=-1)
            process_reward = probs[:, 1] - probs[:, 0]
            step_rewards.append(process_reward)
    if len(step_rewards) != 1:
        raise ValueError(f"Expected single-sample rewards, got {len(step_rewards)}")
    return step_rewards[0]


def bon_score_from_step_reward(step_reward, criterion2, criterion1, temperature=0.1):
    if criterion2 == "softmax":
        weight = torch.softmax(-1 * step_reward / temperature, dim=-1)
        process_reward = weight * step_reward
    elif criterion2 == "plain":
        process_reward = step_reward
    elif criterion2 == "advantage":
        process_reward = step_reward - step_reward.min(-1).values
    else:
        raise ValueError(f"Invalid criterion2: {criterion2}")

    if criterion1 == "last_position":
        final_score = process_reward[-1].cpu()
    elif criterion1 == "min":
        final_score = process_reward.min(-1).values.cpu()
    elif criterion1 == "ave":
        final_score = process_reward.mean(-1).cpu()
    elif criterion1 == "binary":
        final_score = (step_reward > 0).sum(-1).cpu()
    else:
        raise ValueError(f"Invalid criterion1: {criterion1}")
    return final_score


def bon_scores_for_candidates(candidates_dict, criterion2, criterion1, temperature=0.1):
    bon_scores = {}
    for gen_index, single_data in candidates_dict.items():
        step_reward = torch.tensor(single_data["step_reward"])
        final_score = bon_score_from_step_reward(step_reward, criterion2, criterion1, temperature)
        bon_scores[gen_index] = {"bon_score": final_score, **single_data}
    return bon_scores


def calculate_cweval_metrics(bon_scores):
    sorted_candidates = sorted(bon_scores.items(), key=lambda x: x[1]["bon_score"], reverse=True)
    num_candidates = len(sorted_candidates)

    k_values = [1, 3, 5, 10, num_candidates]
    correct = {"functional": {k: 0 for k in k_values}, "secure": {k: 0 for k in k_values}}

    for k in k_values:
        top_k = sorted_candidates[:k]
        if any(c[1]["label_functional"] for c in top_k):
            correct["functional"][k] += 1
        if any(c[1]["label_secure"] for c in top_k):
            correct["secure"][k] += 1

    return correct


def write_cweval_csv(csv_path, run_name, average_metrics, num_groups):
    k_values = sorted(average_metrics["functional"].keys())
    with open(csv_path, "a", newline="") as f:
        writer = csv.writer(f)
        header = ["name", "num_groups"]
        for k in k_values:
            header.append(f"functional@{k}")
            header.append(f"secure@{k}")
        writer.writerow(header)

        row = [run_name, num_groups]
        for k in k_values:
            row.append(average_metrics["functional"][k])
            row.append(average_metrics["secure"][k])
        writer.writerow(row)


def calculate_pass_at_k(bon_scores, k_values):
    sorted_candidates = sorted(bon_scores.items(), key=lambda x: x[1]["bon_score"], reverse=True)
    metrics = {k: 0 for k in k_values}
    for k in k_values:
        top_k = sorted_candidates[:k]
        if any(c[1]["label"] for c in top_k):
            metrics[k] = 1
    return metrics


def calculate_raw_pass_at_k(data, k_values):
    total_metrics = {k: 0 for k in k_values}
    num_questions = 0
    for item in data:
        graded_list = item.get("graded_list", [])
        for k in k_values:
            if any(bool(label) for label in graded_list[:k]):
                total_metrics[k] += 1
        num_questions += 1
    average_metrics = {k: (total_metrics[k] / num_questions if num_questions else 0.0) for k in k_values}
    return average_metrics, num_questions


def write_livecodebench_csv(csv_path, run_name, average_metrics, num_questions):
    k_values = sorted(average_metrics.keys())
    with open(csv_path, "a", newline="") as f:
        writer = csv.writer(f)
        header = ["name", "num_questions"]
        for k in k_values:
            header.append(f"pass@{k}")
        writer.writerow(header)

        row = [run_name, num_questions]
        for k in k_values:
            row.append(average_metrics[k])
        writer.writerow(row)


def _metric_sort_key(metric_key):
    if metric_key == "acc":
        return (0, 0, 0, metric_key)
    match = re.match(r"^(top|best)_(\d+)_", metric_key)
    if match:
        group_order = {"top": 0, "best": 1}
        return (1, int(match.group(2)), group_order.get(match.group(1), 2), metric_key)
    if metric_key.startswith("average"):
        return (2, 0, 0, metric_key)
    return (3, 0, 0, metric_key)


def write_sven_csv(csv_path, run_name, average_metrics):
    metric_keys = sorted(average_metrics.keys(), key=_metric_sort_key)
    with open(csv_path, "a", newline="") as f:
        writer = csv.writer(f)
        header = ["name"]
        header.extend(metric_keys)
        writer.writerow(header)

        row = [run_name]
        for key in metric_keys:
            row.append(average_metrics[key])
        writer.writerow(row)


def run_cweval(args):
    testset = load_cweval_dataset(args.generated_dir)
    prm, prm_tokenizer = build_prm(args.prm_name)

    step_separator = "\n\n"
    step_separator_token = prm_tokenizer(step_separator, add_special_tokens=False, return_tensors="pt")["input_ids"]
    question = "Determine whether the code is vulnerable or not."
    question_ids = prm_tokenizer(question, add_special_tokens=False, return_tensors="pt")["input_ids"]

    for task in testset:
        for language in testset[task]:
            for cwe_id in testset[task][language]:
                subset = testset[task][language][cwe_id]
                for i_th_generated in subset.keys():
                    generated_code = subset[i_th_generated]["code"]
                    step_reward = get_stepwise_rewards(
                        generated_code,
                        step_separator,
                        step_separator_token,
                        question_ids,
                        prm,
                        prm_tokenizer,
                    )
                    subset[i_th_generated]["step_reward"] = step_reward

    average_metrics = {"functional": {}, "secure": {}}
    num_groups = 0
    for task in testset:
        for language in testset[task]:
            for cwe_id in testset[task][language]:
                subset = testset[task][language][cwe_id]
                bon_scores = bon_scores_for_candidates(subset, args.criterion2, args.criterion1)
                metrics = calculate_cweval_metrics(bon_scores)
                for k, v in metrics["functional"].items():
                    average_metrics["functional"][k] = average_metrics["functional"].get(k, 0) + v
                for k, v in metrics["secure"].items():
                    average_metrics["secure"][k] = average_metrics["secure"].get(k, 0) + v
                subset["metrics"] = metrics
                num_groups += 1

    for k in list(average_metrics["functional"].keys()):
        average_metrics["functional"][k] /= num_groups
    for k in list(average_metrics["secure"].keys()):
        average_metrics["secure"][k] /= num_groups

    print("average_metrics:", average_metrics)
    print("num_groups:", num_groups)
    write_cweval_csv(args.csv_path, args.name, average_metrics, num_groups)


def run_livecodebench(args):
    data = load_livecodebench_data(args.input_path)

    k_values = [1, 3, 5]
    raw_average_metrics, _ = calculate_raw_pass_at_k(data, k_values)
    print("raw pass@1:", raw_average_metrics[1])
    print("raw pass@3:", raw_average_metrics[3])
    print("raw pass@5:", raw_average_metrics[5])

    prm, prm_tokenizer = build_prm(args.prm_name)

    step_separator = "\n\n"
    step_separator_token = prm_tokenizer(step_separator, add_special_tokens=False, return_tensors="pt")["input_ids"]

    total_metrics = {k: 0 for k in k_values}
    num_questions = 0

    for item in tqdm(data, desc="Scoring"):
        question = f"{item['question_title']}\n{item['question_content']}".strip()
        question_ids = prm_tokenizer(question, add_special_tokens=False, return_tensors="pt")["input_ids"]

        candidates = {}
        for idx, code in enumerate(item["code_list"]):
            if code is None:
                code = ""
            step_reward = get_stepwise_rewards(
                code,
                step_separator,
                step_separator_token,
                question_ids,
                prm,
                prm_tokenizer,
            )
            candidates[str(idx)] = {"step_reward": step_reward, "label": bool(item["graded_list"][idx])}

        bon_scores = bon_scores_for_candidates(candidates, args.criterion2, args.criterion1)
        metrics = calculate_pass_at_k(bon_scores, k_values)
        for k in k_values:
            total_metrics[k] += metrics[k]
        num_questions += 1

    average_metrics = {k: total_metrics[k] / num_questions for k in k_values}
    print("pass@1:", average_metrics[1])
    print("pass@3:", average_metrics[3])
    print("pass@5:", average_metrics[5])
    write_livecodebench_csv(args.csv_path, args.name, average_metrics, num_questions)


def run_sven(args):
    testset = load_sven_dataset(args.data_dir)
    prm, prm_tokenizer = build_prm(args.prm_name)

    generate_answers = args.generate_answers
    if generate_answers:
        num_gpus = torch.cuda.device_count()
        llm = LLM(
            args.model_name,
            tensor_parallel_size=num_gpus,
            gpu_memory_utilization=0.90,
            max_model_len=8192,
            trust_remote_code=True,
        )
        sampling_params = SamplingParams(temperature=0.8, top_p=0.95, max_tokens=512, n=100)
        evaler = LMCodeEvaler(sampling_params, llm)
        evaler.generate_testset_answers(testset, args.output_dir)

    step_separator = "\n\n"
    step_separator_token = prm_tokenizer(step_separator, add_special_tokens=False, return_tensors="pt")["input_ids"]

    top_k_safe_ratios = {k: [] for k in [1, 3, 5]}
    best_possible_safe_ratios = {k: [] for k in [1, 3, 5]}
    average_safe_ratios = {k: [] for k in [1, 3, 5]}
    acc_scores = []

    for vul_type, data_scenario in testset.items():
        assert len(data_scenario) == 1
        scenario_single_data = data_scenario[0]
        for scenario, _single_data in scenario_single_data.items():
            out_src_dir = os.path.join(args.output_dir, vul_type, scenario, "output")
            question = "Determine whether the code is vulnerable or not."
            question_ids = prm_tokenizer(question, add_special_tokens=False, return_tensors="pt")["input_ids"]
            outcome_scores = []
            labels = []
            true_negative = true_positive = false_negative = false_positive = 0

            for fname in os.listdir(out_src_dir):
                if fname.endswith(".o") or fname.endswith(".py") or "Makefile" in fname:
                    continue
                csv_path = os.path.join(args.output_dir, vul_type, scenario, "codeql.csv")
                label = 1
                with open(csv_path, "r") as f:
                    reader = csv.reader(f)
                    for row in reader:
                        if "/" + fname in row:
                            label = 0
                            break
                labels.append(label)

                with open(os.path.join(out_src_dir, fname), "r") as f:
                    generated_code = f.read()
                for header in [
                    "#include <stdio.h>",
                    "#include <stdlib.h>",
                    "#include <limits.h>",
                    "#include <string.h>",
                    "#include <stdbool.h>",
                ]:
                    generated_code = generated_code.replace(header, "")

                step_reward = get_stepwise_rewards(
                    generated_code,
                    step_separator,
                    step_separator_token,
                    question_ids,
                    prm,
                    prm_tokenizer,
                )

                if args.criterion1 == "acc":
                    pred_or = step_reward.sum().item()
                    print(f"step_reward: {step_reward.tolist()}")
                    print(f"pred_or: {pred_or}, label: {label}")

                    if (label == 0 and pred_or <= 0):
                        true_negative += 1
                    elif (label == 1 and pred_or > 0):
                        true_positive += 1
                    elif (label == 0 and pred_or > 0):
                        false_negative += 1
                    elif (label == 1 and pred_or <= 0):
                        false_positive += 1
                    else:
                        raise ValueError(f"Invalid label: {label}")

                    outcome_reward = 2 * true_positive / (2 * true_positive + false_positive + false_negative)
                    outcome_scores.append(outcome_reward)
                else:
                    outcome_reward = bon_score_from_step_reward(
                        step_reward,
                        args.criterion2,
                        args.criterion1,
                    ).item()
                    outcome_scores.append(outcome_reward)

            def eval_top_k(k):
                top_k_labels = [labels[i] for i in np.argsort(outcome_scores)[-k:]]
                top_k_safe_ratio = np.mean(top_k_labels)
                best_labels = np.sort(labels)[-k:]
                best_possible_safe_ratio = np.mean(best_labels)
                average_safe_ratio = np.mean(labels)
                return top_k_safe_ratio, best_possible_safe_ratio, average_safe_ratio

            if len(outcome_scores) > 0:
                print("--------------------------------")
                if args.criterion1 == "acc":
                    print(
                        f"True_positive: {true_positive}, False_positive: {false_positive}, "
                        f"True_negative: {true_negative}, False_negative: {false_negative}"
                    )
                    if true_positive + false_positive != 0:
                        print(f"Precision: {true_positive/(true_positive + false_positive)}")
                    if true_positive + false_negative != 0:
                        print(f"Recall: {true_positive/(true_positive + false_negative)}")
                    if 2 * true_positive + false_positive + false_negative != 0:
                        print(f"F1: {2*true_positive/(2*true_positive + false_positive + false_negative)}")
                    else:
                        print("F1: 0")
                    acc_scores.extend(outcome_scores)
                else:
                    for k in [1, 3, 5]:
                        top_k_safe_ratio, best_possible_safe_ratio, average_safe_ratio = eval_top_k(k)
                        top_k_safe_ratios[k].append(top_k_safe_ratio)
                        best_possible_safe_ratios[k].append(best_possible_safe_ratio)
                        average_safe_ratios[k].append(average_safe_ratio)
                        print(
                            f"top_{k}_safe_ratio: {top_k_safe_ratio}, "
                            f"best_possible_safe_ratio: {best_possible_safe_ratio}, "
                            f"average_safe_ratio: {average_safe_ratio}"
                        )

                with open(os.path.join(args.output_dir, vul_type, scenario, "result.jsonl"), "r") as f:
                    for line in f:
                        print(f"{vul_type}{scenario}:", line)

    print("--------------------------------")
    average_metrics = {}
    if args.criterion1 != "acc":
        for k in [1, 3, 5]:
            average_metrics[f"top_{k}_safe_ratios"] = np.mean(top_k_safe_ratios[k])
            average_metrics[f"best_{k}_possible_safe_ratios"] = np.mean(best_possible_safe_ratios[k])
            average_metrics[f"average_safe_ratios"] = np.mean(average_safe_ratios[k])
    else:
        average_metrics["acc"] = np.mean(acc_scores)
    write_sven_csv(args.csv_path, args.name, average_metrics)


def build_parser():
    parser = argparse.ArgumentParser(description="Unified BoN ranking runner")
    subparsers = parser.add_subparsers(dest="command", required=True)

    cweval = subparsers.add_parser("cweval", help="Run CWEval ranking")
    cweval.add_argument("--criterion2", choices=["softmax", "plain", "advantage"], default="softmax")
    cweval.add_argument("--criterion1", choices=["binary", "min", "ave", "last_position"], default="ave")
    cweval.add_argument("--prm_name", default="vivi-yu/sven_s2_f1_71")
    cweval.add_argument(
        "--generated_dir",
        default="/project/flame/wyu3/PRM/bon/CWEval/evals/eval_251001_225018",
    )
    cweval.add_argument("--name", default="run")
    cweval.add_argument("--csv_path", default="cweval_exp.csv")

    live = subparsers.add_parser("livecodebench", help="Run LiveCodeBench ranking")
    live.add_argument(
        "--input_path",
        default="/home/wyu3/workspace/LiveCodeBench/output/Qwen2.5-Coder-Ins-7B/Scenario.codegeneration_10_0.2_eval_all.json",
    )
    live.add_argument("--criterion2", choices=["softmax", "plain", "advantage"], default="softmax")
    live.add_argument("--criterion1", choices=["binary", "min", "ave", "last_position"], default="ave")
    live.add_argument("--prm_name", default="vivi-yu/sven_s2_f1_71")
    live.add_argument("--name", default="run")
    live.add_argument("--csv_path", default="livecodebench_exp.csv")

    sven = subparsers.add_parser("sven", help="Run SVEN ranking")
    sven.add_argument("--model_name", default="Qwen/Qwen2.5-7B-Instruct")
    sven.add_argument("--output_dir", default="/project/flame/wyu3/PRM/sven_trained/")
    sven.add_argument("--data_dir", default="/project/flame/wyu3/PRM/sven_git/sven/data_eval/trained")
    sven.add_argument("--criterion2", choices=["softmax", "plain", "advantage"], default="softmax")
    sven.add_argument("--criterion1", choices=["binary", "min", "ave", "last_position", "acc"], default="binary")
    sven.add_argument("--prm_name", default="vivi-yu/precise_s1")
    sven.add_argument("--generate_answers", action="store_true")
    sven.add_argument("--name", default="test")
    sven.add_argument("--csv_path", default="sven_exp.csv")

    return parser


def main():
    parser = build_parser()
    args = parser.parse_args()

    if args.command == "cweval":
        run_cweval(args)
    elif args.command == "livecodebench":
        run_livecodebench(args)
    elif args.command == "sven":
        run_sven(args)
    else:
        raise ValueError(f"Unknown command: {args.command}")


if __name__ == "__main__":
    main()
