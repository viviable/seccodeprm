from datasets import load_from_disk
from vllm import LLM, SamplingParams

def extract_label(output):
    if "yes" in output.lower():
        return 1
    elif "no" in output.lower():
        return 0
    else:
        return -1

def main():
    dataset = load_from_disk("/project/flame/wyu3/PRM/bigvul_processed_dataset")["test"]
    model_path = "Qwen/Qwen2.5-Coder-7B-Instruct"
    llm = LLM(
        model=model_path,
        tensor_parallel_size=1,
        gpu_memory_utilization=0.9,
        max_model_len=4096,
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
    batch_size = 16
    results = []
    for i in range(0, len(prompts), batch_size):
        prompts_batch = prompts[i:i+batch_size]
        labels_batch = labels[i:i+batch_size]
        outputs = llm.generate(prompts_batch, sampling_params)
        for output in outputs:
            vul = extract_label(output)
            results.append(output)
            if vul == labels_batch[i]:
                acc += 1
                
    acc = acc / len(results)
    
    
    

if __name__ == "__main__":
    main()
