from datasets import load_from_disk, load_dataset
from vllm import LLM, SamplingParams
import numpy as np
from tqdm import tqdm
from transformers import AutoTokenizer

def extract_label(output):
    if "yes" in output.lower():
        return 0
    elif "no" in output.lower():
        return 1
    else:
        return -1

def main():
    # dataset = load_from_disk('/project/flame/wyu3/PRM/bigvul_processed_dataset_dedup_test')
    # dataset = dataset.select(range(10))
    dataset = load_dataset('vivi-yu/bigvul_dedup_test')['train']
    model_name = "openai/gpt-oss-20b"
    tokenizer = AutoTokenizer.from_pretrained(model_name)
    llm = LLM(model=model_name, tensor_parallel_size=1)
    sampling_params = SamplingParams(max_tokens=131072)


    # 推理
    prompts = []
    labels = []
    messages = []
    texts = []
    for data in dataset:
        for i in range(len(data["completions"])):
            prompt =  f'Given the previous code {data["completions"][:i]}, determine whether the current code {data["completions"][i]} is vulnerable or not. Answer with Yes or No.'
            prompts.append(prompt)
            labels.append(data["labels"][i])
            
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
            
    for answers in [answer_medium, answer_high, answer_low]:
        acc_safe = 0
        acc_vul = 0
        invalid = 0
        
        for i in tqdm(range(0, len(prompts))):
            output_text = answers[i]
            safe = extract_label(output_text)
            if safe == 1 and labels[i] == 1:
                acc_safe += 1
            elif safe == 0 and labels[i] == 0:
                acc_vul += 1
            elif safe == -1:
                invalid += 1
                    
        acc = (acc_safe + acc_vul) / len(prompts)
        print(f"Accuracy: {acc}")
        print(f"Total number of results: {len(prompts)}")
        print(f"Accuracy of safe: {acc_safe / (np.array(labels)==1).sum()}")
        print(f"Accuracy of vul: {acc_vul / (np.array(labels)==0).sum()}")
        print(f"Invalid: {invalid / len(prompts)}")
        
        with open(f"baseline_llm_prompting_oss_{answers}.txt", "w") as f:
            for result in answers:
                f.write(result + "\n")
    

if __name__ == "__main__":
    main()










