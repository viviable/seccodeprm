import numpy as np
import torch
from datasets import load_from_disk
from tqdm import tqdm
from transformers import AutoModelForTokenClassification, AutoTokenizer
import os
import json
import re
import ast
import subprocess
from vllm import LLM
from constant import CWES_TRAINED


def load_n_prepare_dataset(ds_name):
    
    if ds_name == "sven":
        data_dir = '/home/wyu3/workspace/sven/data_eval/trained'
        output_dir = '/home/wyu3/workspace/PURE/PRM/eval/sven_trained'
        testset = {}
        vul_types = CWES_TRAINED
        
        for vul_type in vul_types:
            testset[vul_type] = []
            vul_dir = os.path.join(data_dir, vul_type)
            output_dir = os.path.join(output_dir, vul_type)
            os.makedirs(output_dir, exist_ok=True)
            
            new_data = {}
            for scenario in list(sorted(os.listdir(vul_dir))):
                new_data[scenario] = {}
                with open(os.path.join(vul_dir, scenario, 'info.json')) as f:
                    info = json.load(f)
                new_data[scenario]['info'] = info
                with open(os.path.join(vul_dir, scenario, 'file_context.'+info['language'])) as f:
                    file_context = f.read()
                with open(os.path.join(vul_dir, scenario, 'func_context.'+info['language'])) as f:
                    func_context = f.read()
                new_data[scenario]['file_context'] = file_context
                new_data[scenario]['func_context'] = func_context
                testset[vul_type].append(new_data)
            
        return testset
    
class LEvaler:
    def __init__(self, model_name):
        self.model_name = model_name
        self.tokenizer = AutoTokenizer.from_pretrained(model_name)
        self.model = AutoModelForTokenClassification.from_pretrained(model_name)
        self.model.eval()
        self.device = self.model.device
    def truncate(self, completion, lang):
        if lang == 'py':
            for match in re.finditer('\n', completion):
                cur_idx, next_idx = match.start(), match.end()
                if next_idx < len(completion) and not completion[next_idx].isspace():
                    completion = completion[:cur_idx]
                    break
            else:
                last_comment_str = '\n    #'
                if last_comment_str in completion:
                    completion = completion[:completion.rfind(last_comment_str)]
        elif lang == 'c':
            if '\n}' in completion:
                completion = completion[:completion.find('\n}')+2]
            else:
                last_comment_strs = ['\n    //', '\n    /*']
                for last_comment_str in last_comment_strs:
                    if last_comment_str in completion:
                        completion = completion[:completion.rfind(last_comment_str)]
                        completion = completion.rstrip() + '\n}'

            lines = completion.split('\n')
            final_lines = []
            for line in lines:
                if '->name = "' in line: continue
                final_lines.append(line)
            completion = '\n'.join(final_lines)
        else:
            raise NotImplementedError()

        return completion  
    
    def try_parse(self, code, lang):
        if lang == 'py':
            try:
                ast.parse(code)
                return 0
            except:
                return 1
        elif lang == 'c':
            cmd = 'gcc -c -x c -'
            process = subprocess.run(cmd, shell=True, timeout=5, input=code.encode(), stderr=subprocess.DEVNULL)
            if process.returncode == 0:
                return 0
            else:
                return 1
        else:
            raise NotImplementedError()
    
    def process_completions(self, input_src, input_ids_len, gen_output, lang):
        tokens = gen_output[:, input_ids_len:, ...]
        completions = self.tokenizer.batch_decode(tokens)

        output_srcs, output_ids = [], []
        dup_srcs, non_parsed_srcs = [], []
        for i, completion in enumerate(completions):
            if self.tokenizer.eos_token in completion:
                completion = completion[:completion.find(self.tokenizer.eos_token)]
            completion = self.truncate(completion, lang)
            completion_len = len(self.tokenizer.encode(completion))
            output_src = input_src + completion
            output_src = output_src.rstrip() + '\n'
            if output_src in output_srcs:
                dup_srcs.append(output_src)
            elif self.try_parse(output_src, lang) != 0:
                non_parsed_srcs.append(output_src)
            else:
                output_srcs.append(output_src)
                output_ids.append((gen_output[i][:input_ids_len].tolist(), gen_output[i][input_ids_len:input_ids_len+completion_len].tolist()))

        return output_srcs, output_ids, dup_srcs, non_parsed_srcs
               
    def generate_testset_answers(self, testset, num_return_sequences):
        device = self.device
        results = {}
        for vul_type in testset:
            results[vul_type] = []
            results[vul_type]['vul_type'] = vul_type
            
            for data_scenario in testset[vul_type]:
                file_context = data_scenario['file_context']
                func_context = data_scenario['func_context']
                lang = data_scenario['info']['language']
                
                input_src = file_context + func_context
                input_ids = self.tokenizer(input_src, return_tensors='pt').input_ids.to(device)
                input_ids_len = input_ids.shape[1]
                gen_output = self.model.generate(
                    input_ids,
                    do_sample=True,
                    num_return_sequences=num_return_sequences,
                    temperature=0.1,
                    max_new_tokens=1024,
                )
                output_srcs, output_ids, dup_srcs, non_parsed_srcs = self.process_completions(input_src, input_ids_len, gen_output, lang)
                data_scenario['output_srcs'] = output_srcs
                data_scenario['output_ids'] = output_ids
                data_scenario['dup_srcs'] = dup_srcs
                data_scenario['non_parsed_srcs'] = non_parsed_srcs
                results[vul_type].append(data_scenario)
                
        return results
            
            
        

def make_step_rewards(logits, token_masks):
    all_scores_res = []
    for sample, token_mask in zip(logits, token_masks):
        # sample: (seq_len, num_labels)
        probs = sample[token_mask].softmax(dim=-1)  # (num_steps, 2)
        process_reward = probs[:, 1] - probs[:, 0]  # (num_steps,)
        # weighted sum to approx. min, highly recommend when BoN eval and Fine-tuning LLM
        weight = torch.softmax(
            -process_reward / 0.1, 
            dim=-1,
        )
        process_reward = weight * process_reward
        all_scores_res.append(process_reward.cpu().tolist())
    return all_scores_res

def main():
    testset = load_n_prepare_dataset("sven")
    import pdb; pdb.set_trace()
    model_name = "Qwen/Qwen2.5-Math-PRM-7B"
    device = "auto"

    tokenizer = AutoTokenizer.from_pretrained(
        model_name, 
        trust_remote_code=True,
    )
    # model = AutoModelForTokenClassification.from_pretrained(
    #     model_name, 
    #     device_map=device, 
    #     torch_dtype=torch.bfloat16,
    #     trust_remote_code=True,
    # ).eval()

    llm = LLM(model_name, 
              tensor_parallel_size=1, 
              gpu_memory_utilization=0.95, 
              max_log_tokens=1024,
              trust_remote_code=True)
    step_separator = "\n\n"
    step_separator_token = tokenizer(
        step_separator, 
        add_special_tokens=False, 
        return_tensors='pt',
    )['input_ids']


    for ds_item, ds_name in zip(ds, ds_names):
        # sampled_ids = np.random.choice(range(len(ds_item)), size=100, replace=False)
        correct = 0
        total = 0
        for idx in tqdm(range(len(ds_item)), desc=f"Processing questions in {ds_name}"):
            question = ds_item['prompt'][idx]
            answers = ds_item['answers'][idx]
            labels = ds_item['label'][idx]
            outcome_scores = []

            question_ids = tokenizer(
                question, 
                add_special_tokens=False, 
                return_tensors='pt',
            )['input_ids']
            for answer in tqdm(answers, desc="Processing answers"):
                steps = [i.rstrip() for i in answer.split("\n\n")]
                input_ids = question_ids.clone()

                score_ids = []
                for step in steps:
                    step_ids = tokenizer(
                        step, 
                        add_special_tokens=False, 
                        return_tensors='pt',
                    )['input_ids']
                    input_ids = torch.cat(
                        [input_ids, step_ids, step_separator_token], 
                        dim=-1,
                    )
                    score_ids.append(input_ids.size(-1) - 1)

                input_ids = input_ids.to(model.device, dtype=torch.long)
                token_masks = torch.zeros_like(input_ids, dtype=torch.bool)
                token_masks[0, score_ids] = True
                assert torch.all(input_ids[token_masks].to("cpu") == step_separator_token)
                
                with torch.no_grad():
                    logits = model(input_ids).logits
                    step_reward = make_step_rewards(logits, token_masks)
                    outcome_reward = torch.tensor(step_reward).sum(dim=-1)

                # TODO: batch input & output
                outcome_scores.append(outcome_reward.item())
            
            best_idx = np.argmax(outcome_scores)
            if labels[best_idx] == 1:
                correct += 1
            total += 1
        print(f"Accuracy on {ds_name}: {correct / total}")

if __name__ == "__main__":
    main()