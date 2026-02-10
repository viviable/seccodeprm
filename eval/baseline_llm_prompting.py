from datasets import load_from_disk, load_dataset
from vllm import LLM, SamplingParams
from transformers import AutoTokenizer
import numpy as np
from tqdm import tqdm
import torch
import json
import os

datasets = {
    "precisebugs_test": load_dataset("vivi-yu/vul_code_precise")["test"],
    # "reposvul_test": load_dataset("vivi-yu/reposvul_processed_dataset")["test"],
    # "sven_test": load_dataset("vivi-yu/vul_code_sven")["val"],
    # "primevul_test_paired": load_dataset("vivi-yu/primevul_processed_dataset")["test"],
    # "primevul_test_unpaired": load_dataset("vivi-yu/primevul_processed_dataset_unpaired")["test"],
}


def extract_label(output):
    if "yes" in output.lower():
        return 0
    elif "no" in output.lower():
        return 1
    else:
        return -1

def api_call():
    # Minimal Bedrock text completion for Claude-style models.
    # Requires AWS credentials (env or config) and Bedrock access.
    def _call(prompt, model_id="anthropic.claude-3-5-sonnet-20240620-v1:0",
              max_tokens=256, temperature=0.0, top_p=1.0, region=None):
        import boto3

        region_name = region or os.environ.get("AWS_REGION") or os.environ.get("AWS_DEFAULT_REGION") or "us-east-1"
        client = boto3.client("bedrock-runtime", region_name=region_name)

        if model_id.startswith("anthropic."):
            body = {
                "anthropic_version": "bedrock-2023-05-31",
                "messages": [{"role": "user", "content": prompt}],
                "max_tokens": max_tokens,
                "temperature": temperature,
                "top_p": top_p,
            }
        elif model_id.startswith("amazon.titan-text"):
            body = {
                "inputText": prompt,
                "textGenerationConfig": {
                    "maxTokenCount": max_tokens,
                    "temperature": temperature,
                    "topP": top_p,
                },
            }
        else:
            raise ValueError(f"Unsupported Bedrock model_id: {model_id}")

        response = client.invoke_model(
            modelId=model_id,
            body=json.dumps(body),
            contentType="application/json",
            accept="application/json",
        )
        payload = json.loads(response["body"].read().decode("utf-8"))

        if model_id.startswith("anthropic."):
            return payload["content"][0]["text"]
        if model_id.startswith("amazon.titan-text"):
            return payload["results"][0]["outputText"]
        return ""

    return _call
    
def main():
    # model_path = "Qwen/Qwen2.5-Coder-7B-Instruct"
    model_path = "Qwen/Qwen2.5-Coder-32B-Instruct"
    # model_path = "Qwen/Qwen3-Coder-30B-A3B-Instruct"
    # model_path = "mistralai/Codestral-22B-v0.1"
    # model_path = "meta-llama/Llama-4-Scout-17B-16E-Instruct"
    # model_path = "google/gemma-3-12b-it"
    
    available_gpus = [f"cuda:{i}" for i in range(torch.cuda.device_count())]
    tensor_parallel_size = len(available_gpus)

        
    llm = LLM(
        model=model_path,
        tensor_parallel_size=tensor_parallel_size,
        gpu_memory_utilization=0.9,
        max_model_len=32768,
    )
    tokenizer = AutoTokenizer.from_pretrained(model_path)
    sampling_params = SamplingParams(temperature=0.8, top_p=0.95)

    for dataset_name in datasets:
        dataset = datasets[dataset_name]
        dataset = dataset.select(range(100))
        # 推理
        prompts = []
        labels = []
        for data in dataset:
            for i in tqdm(range(len(data["completions"]))):
                prompt =  f'Given the previous code {data["completions"][:i]}, determine whether the current code {data["completions"][i]} is vulnerable or not. Answer with Yes or No.'
                tokens = tokenizer.encode(prompt)
                if len(tokens) > 30000:
                    print(f"Prompt too long: {len(tokens)}")
                    continue
                prompts.append(prompt)
                labels.append(data["labels"][i])
        print('dataset_name:', dataset_name)
        print(f"Total prompts: {len(prompts)}")
        invalid = 0
        batch_size = 64
        results = []
        TP = 0
        FP = 0
        TN = 0
        FN = 0
        
        for i in range(0, len(prompts), batch_size):
            max_len = min(i+batch_size, len(prompts))
            prompts_batch = prompts[i:max_len]
            labels_batch = labels[i:max_len]
            try:
                outputs = llm.generate(prompts_batch, sampling_params)
                for j in range(max_len - i):
                    output_text = outputs[j].outputs[0].text
                    output_text = output_text.split("assistantfinal:")[-1].lower()
                    safe = extract_label(output_text)
                    results.append(output_text)
                    if safe == 1 and labels_batch[j] == 1:
                        TN += 1
                    elif safe == 0 and labels_batch[j] == 0:
                        TP += 1
                    elif safe == 0 and labels_batch[j] == 1:
                        FN += 1
                    elif safe == 1 and labels_batch[j] == 0:
                        FP += 1
                    elif safe == -1:
                        invalid += 1
            except Exception as e:
                print(f"Error: {e}")
                continue
            
        print('='*20)
        print(f"name: {dataset_name}")
        print(f"TP: {TP}, FP: {FP}, TN: {TN}, FN: {FN}")
        if TP + FP == 0:
            print("Precision is undefined")
        else:
            print(f"Precision: {TP / (TP + FP)}")
        if TP + FN == 0:
            print("Recall is undefined")
        else:
            print(f"Recall: {TP / (TP + FN)}")

        print(f"Accuracy: {(TP + TN) / (TP + TN + FP + FN)}")
        if 2 * TP + FP + FN == 0:
            print("F1-score is undefined")
        else:
            print(f"F1-score: {2 * TP / (2 * TP + FP + FN)}")
        print(f"Total number of results: {len(results)}")
        print(f"Invalid: {invalid / len(results)}")
        
        with open(f"baseline_llm_prompting_{model_path.replace('/', '_')}_{dataset_name}.txt", "a") as f:
            for result in results:
                f.write(result + "\n")

if __name__ == "__main__":
    main()
