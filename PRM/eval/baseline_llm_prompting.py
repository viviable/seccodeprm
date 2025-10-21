from datasets import load_from_disk
from vllm import LLM, SamplingParams
from transformers import AutoTokenizer
import numpy as np
from tqdm import tqdm

datasets = {
    # "precisebugs_test": load_from_disk("/project/flame/wyu3/PRM/precisebugs_processed_dataset")["test"],
    "reposvul_test": load_from_disk("/project/flame/wyu3/PRM/reposvul_processed_dataset")["test"],
    # "sven_test": load_from_disk("/project/flame/wyu3/PRM/sven_processed_dataset")["test"],
    # "bigvul_test": load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset")["test"],
    # "bigvul_dedup_test": load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset_dedup_test_dedup"),
    # "primevul_test_paired": load_from_disk("/project/flame/wyu3/PRM/primevul_processed_dataset")["test"],
    # "primevul_test_unpaired": load_from_disk("/project/flame/wyu3/PRM/primevul_processed_dataset_unpaired")["test"],
}

# datasets = {
#     "precisebugs_test": load_dataset("vivi-yu/vul_code_precise")["test"],
#     "reposvul_test": load_dataset("vivi-yu/vul_code_repos")["test"],
#     "sven_test": load_dataset("vivi-yu/vul_code_sven")["test"],
#     "bigvul_dedup_test": load_dataset("vivi-yu/vul_code_bigvul_dedup")["train"],
#     "primevul_test_paired": load_dataset("vivi-yu/vul_code_primevul")["test"],
#     "primevul_test_unpaired": load_dataset("vivi-yu/vul_code_primevul_unpaired")["test"],
# }

def extract_label(output):
    if "yes" in output.lower():
        return 0
    elif "no" in output.lower():
        return 1
    else:
        return -1

def main():
    
    # model_path = "Qwen/Qwen2.5-Coder-32B-Instruct"
    model_path = "Qwen/Qwen3-Coder-30B-A3B-Instruct"
    # model_path = "Codestral"
    
    
    if "32b" or "30b" in model_path.lower():
        tensor_parallel_size = 4
    else:
        tensor_parallel_size = 1
        
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
            
            
                    
        print(f"TP: {TP}, FP: {FP}, TN: {TN}, FN: {FN}")
        print(f"Precision: {TP / (TP + FP)}")
        print(f"Recall: {TP / (TP + FN)}")
        print(f"Accuracy: {(TP + TN) / (TP + TN + FP + FN)}")
        print(f"F1-score: {2 * TP / (2 * TP + FP + FN)}")
        print(f"Total number of results: {len(results)}")
        print(f"Invalid: {invalid / len(results)}")
        
        with open(f"baseline_llm_prompting_{model_path.replace('/', '_')}_{dataset_name}.txt", "a") as f:
            for result in results:
                f.write(result + "\n")

if __name__ == "__main__":
    main()
