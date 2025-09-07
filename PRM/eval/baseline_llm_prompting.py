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
    dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset")["test"]
    # dataset = dataset.select(range(10))
    model_path = "Qwen/Qwen2.5-Coder-7B-Instruct"
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
    acc_safe = 0
    acc_vul = 0
    invalid = 0
    batch_size = 64
    results = []
    
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
                    acc_safe += 1
                elif safe == 0 and labels_batch[j] == 0:
                    acc_vul += 1
                elif safe == -1:
                    invalid += 1
        except Exception as e:
            print(f"Error: {e}")
            continue
        
        
                
    acc = (acc_safe + acc_vul) / len(results)
    print(f"Accuracy: {acc}")
    print(f"Total number of results: {len(results)}")
    print(f"Accuracy of safe: {acc_safe / (np.array(labels)==1).sum()}")
    print(f"Accuracy of vul: {acc_vul / (np.array(labels)==0).sum()}")
    print(f"Invalid: {invalid / len(results)}")
    
    with open("baseline_llm_prompting_1k.txt", "w") as f:
        for result in results:
            f.write(result + "\n")
    
    
    

if __name__ == "__main__":
    main()
