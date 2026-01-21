import argparse
import csv
import json
import torch
from tqdm import tqdm
from transformers import AutoTokenizer, AutoModelForTokenClassification, AutoModel


def load_data(input_path):
    with open(input_path, "r") as f:
        return json.load(f)


def bon(candidates_dict, criterion2, criterion1, temperature=0.1):
    bon_scores = {}
    for gen_index, single_data in candidates_dict.items():
        step_reward = torch.tensor(single_data["step_reward"])
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

        bon_scores[gen_index] = {
            "bon_score": final_score,
            "label": single_data["label"],
        }
    return bon_scores


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
    average_metrics = {
        k: (total_metrics[k] / num_questions if num_questions else 0.0) for k in k_values
    }
    return average_metrics, num_questions


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


def write_metrics_csv(csv_path, run_name, average_metrics, num_questions):
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


def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--input_path",
        type=str,
        default="/home/wyu3/workspace/LiveCodeBench/output/Qwen2.5-Coder-Ins-7B/Scenario.codegeneration_10_0.2_eval_all.json",
    )
    parser.add_argument("--criterion2", type=str, choices=["softmax", "plain", "advantage"], default="softmax")
    parser.add_argument(
        "--criterion1",
        type=str,
        choices=["binary", "min", "ave", "last_position"],
        default="ave",
    )
    parser.add_argument("--prm_name", type=str, default="vivi-yu/sven_s2_f1_71")
    parser.add_argument("--name", type=str, default="run")
    parser.add_argument("--csv_path", type=str, default="livecodebench_exp.csv")
    return parser.parse_args()


def main():
    args = get_args()
    data = load_data(args.input_path)

    k_values = [1, 3, 5]
    raw_average_metrics, _ = calculate_raw_pass_at_k(data, k_values)
    print("raw pass@1:", raw_average_metrics[1])
    print("raw pass@3:", raw_average_metrics[3])
    print("raw pass@5:", raw_average_metrics[5])

    device = "auto"
    if "qwen" in args.prm_name.lower():
        prm = AutoModel.from_pretrained(
            args.prm_name,
            device_map=device,
            dtype=torch.bfloat16,
            trust_remote_code=True,
        ).eval()
    else:
        prm = AutoModelForTokenClassification.from_pretrained(
            args.prm_name,
            device_map=device,
            torch_dtype=torch.bfloat16,
            trust_remote_code=True,
        ).eval()
    prm_tokenizer = AutoTokenizer.from_pretrained(args.prm_name, trust_remote_code=True)

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
            candidates[str(idx)] = {
                "step_reward": step_reward,
                "label": bool(item["graded_list"][idx]),
            }

        bon_scores = bon(candidates, criterion2=args.criterion2, criterion1=args.criterion1)
        metrics = calculate_pass_at_k(bon_scores, k_values)
        for k in k_values:
            total_metrics[k] += metrics[k]
        num_questions += 1

    average_metrics = {k: total_metrics[k] / num_questions for k in k_values}
    print("pass@1:", average_metrics[1])
    print("pass@3:", average_metrics[3])
    print("pass@5:", average_metrics[5])
    write_metrics_csv(args.csv_path, args.name, average_metrics, num_questions)


if __name__ == "__main__":
    main()
