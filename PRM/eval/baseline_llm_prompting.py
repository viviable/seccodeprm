from datasets import load_from_disk
from vllm import LLM, SamplingParams
import numpy as np
from tqdm import tqdm

def extract_label(output):
    if "yes" in output.lower():
        return 0
    elif "no" in output.lower():
        return 1
    else:
        return -1

def main():
    # dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset")["test"]
    dataset = load_from_disk("/project/flame/wyu3/PRM/primevul_processed_dataset_unpaired")["test"]
    
    # dataset = dataset.select(range(10))
    model_path = "Qwen/Qwen2.5-Coder-32B-Instruct"
    llm = LLM(
        model=model_path,
        tensor_parallel_size=1,
        gpu_memory_utilization=0.9,
        max_model_len=32768,
    )
    sampling_params = SamplingParams(temperature=0.8, top_p=0.95)

    # 推理
    prompts = []
    labels = []
    for data in dataset:
        for i in range(len(data["completions"])):
            prompt =  f'Given the previous code {data["completions"][:i]}, determine whether the current code {data["completions"][i]} is vulnerable or not. Answer with Yes or No.'
            prompts.append(prompt)
            labels.append(data["labels"][i])
            
    print(f"Total prompts: {len(prompts)}")
    invalid = 0
    batch_size = 64
    results = []
    TP = 0
    FP = 0
    TN = 0
    FN = 0
    
    for i in tqdm(range(0, len(prompts), batch_size)):
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
    
    with open("baseline_llm_prompting_prime_test.txt", "w") as f:
        for result in results:
            f.write(result + "\n")
    
    
    

if __name__ == "__main__":
    main()
