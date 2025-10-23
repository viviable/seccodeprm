from datasets import load_from_disk, load_dataset
from vllm import LLM, SamplingParams
import numpy as np
from tqdm import tqdm
from transformers import AutoTokenizer
import argparse

datasets = {
    "precisebugs_test": load_dataset("vivi-yu/vul_code_precise")["test"],
    "reposvul_test": load_dataset("vivi-yu/reposvul_processed_dataset")["test"],
    "sven_test": load_dataset("vivi-yu/vul_code_sven")["val"],
    "bigvul_dedup_test": load_dataset("vivi-yu/bigvul_dedup_test")["train"],
    "primevul_test_paired": load_dataset("vivi-yu/primevul_processed_dataset")["test"],
    "primevul_test_unpaired": load_dataset("vivi-yu/primevul_processed_dataset_unpaired")["test"],
}

def extract_label(output):
    output = output.split("assistantfinal:")[-1].lower()
    if "yes" in output:
        return 0
    elif "no" in output:
        return 1
    else:
        return -1

def arg_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("--criteria", type=str, default="steps-level", choices=["steps-level", "sample-level"])
    return parser.parse_args()

def main():
    
    args = arg_parser()
    model_name = "openai/gpt-oss-120b"
    tokenizer = AutoTokenizer.from_pretrained(model_name)
    llm = LLM(model=model_name, tensor_parallel_size=4)
    sampling_params = SamplingParams(max_tokens=131072)
    
    criteria = args.criteria
    
    for dataset_name in datasets:
        dataset = datasets[dataset_name]
        dataset = dataset.select(range(100))
        # 推理
        prompts = []
        labels = []
        messages = []
        texts = []
        for data in dataset:
            if criteria == "steps-level":
                for i in range(len(data["completions"])):
                    prompt =  f'Given the previous code {data["completions"][:i]}, determine whether the current code {data["completions"][i]} is vulnerable or not. Answer with Yes or No.'
                    prompts.append(prompt)
                    labels.append(data["labels"][i])
            elif criteria == "sample-level":
                prompt =  f'Given the previous code {data["completions"][:i]}, determine whether the current code {data["completions"][i]} is vulnerable or not. Answer with Yes or No.'
                prompts.append(prompt)
                if 0 in data["labels"]:
                    labels.append(0)
                else:
                    labels.append(1)
                

            messages = [
                [{"role": "user", "content": prompt}]
                for prompt in prompts
            ]
            texts = [
                tokenizer.apply_chat_template(
                    message,
                    tokenize=False,
                    add_generation_prompt=True,
                )
                for message in messages
            ]
            for text in texts:
                if tokenizer(text)["input_length"] > 131072:
                    texts.remove(text)
                    

            output = llm.generate(texts, sampling_params)
            answer_medium = [i.outputs[0].text for i in output]

            output = llm.generate(
                [text.replace("Reasoning: medium", "Reasoning: high") for text in texts],
                sampling_params,
            )

            answer_high = [i.outputs[0].text for i in output]


            output = llm.generate(
                [text.replace("Reasoning: medium", "Reasoning: low") for text in texts],
                sampling_params,
            )

            answer_low = [i.outputs[0].text for i in output]

            names = ["answer_medium", "answer_high", "answer_low"]
            for answers, name in zip([answer_medium, answer_high, answer_low], names):
                TP = 0
                FP = 0
                TN = 0
                FN = 0
                invalid = 0
                results = []
                for i in tqdm(range(0, len(prompts))):
                    output_text = answers[i]
                    safe = extract_label(output_text)
                    results.append(output_text)
                    
                    if safe == 1 and labels[i] == 1:
                        TN += 1
                    elif safe == 0 and labels[i] == 0:
                        TP += 1
                    elif safe == 0 and labels[i] == 1:
                        FN += 1
                    elif safe == 1 and labels[i] == 0:
                        FP += 1
                    elif safe == -1:
                        invalid += 1
                print(f"name: {dataset_name}_{name}")
                print(f"TP: {TP}, FP: {FP}, TN: {TN}, FN: {FN}")
                if TP + FP == 0:
                    print("Precision is undefined")
                else:
                    print(f"Precision: {TP / (TP + FP)}")
                if TP + FN == 0:
                    print("Recall is undefined")
                else:
                    print(f"Recall: {TP / (TP + FN)}")
                if TP + TN + FP + FN == 0:
                    print("Accuracy is undefined")
                else:
                    print(f"Accuracy: {(TP + TN) / (TP + TN + FP + FN)}")
                if 2 * TP + FP + FN == 0:
                    print("F1-score is undefined")
                else:
                    print(f"F1-score: {2 * TP / (2 * TP + FP + FN)}")
                print(f"Total number of results: {len(results)}")
                print(f"Invalid: {invalid / len(results)}")

            with open(f"baseline_llm_prompting_oss_{name}_{dataset_name}.txt", "w") as f:
                for result in answers:
                    f.write(result + "\n")


if __name__ == "__main__":
    main()
