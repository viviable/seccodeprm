import math

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
    parser.add_argument("--criteria", type=str, default="steps-level",
                        choices=["steps-level", "sample-level"])
    parser.add_argument("--max_input_tokens", type=int, default=None,
                        help="最大输入 token（不含生成），缺省则按模型上限与max_gen_tokens自动计算")
    parser.add_argument("--max_gen_tokens", type=int, default=2048,
                        help="每条生成的最大token数，传给 SamplingParams(max_tokens=...)")
    parser.add_argument("--truncate_strategy", type=str, default="tail",
                        choices=["tail", "headtail", "steps-last-k"],
                        help="超长时的截断策略")
    parser.add_argument("--steps_k", type=int, default=5,
                        help="steps-last-k 策略下保留最近K个 step")
    parser.add_argument("--head_tokens", type=int, default=512,
                        help="headtail 策略下保留的头部token数（尾部=剩余给尾部）")
    parser.add_argument("--tail_tokens", type=int, default=4096,
                        help="headtail 策略下保留的尾部token数（若与head_tokens冲突会自动收缩）")
    return parser.parse_args()

def count_tokens(text, tokenizer):
    return len(tokenizer(text)["input_ids"])

def truncate_text_to_tokens(text, tokenizer, max_tokens, strategy="tail",
                            head_tokens=512, tail_tokens=4096):
    """按 token 计数对 text 截断，并返回截断后的 text。"""
    ids = tokenizer(text)["input_ids"]
    if len(ids) <= max_tokens:
        return text
    if strategy == "tail":
        ids = ids[-max_tokens:]
    elif strategy == "headtail":
        # 分配头尾；若head+tail>max_tokens则按比例收缩尾部
        h = min(head_tokens, max_tokens // 2)
        t = max_tokens - h
        # 如果指定了 tail_tokens，就尝试满足；不足则以可用t为准
        t = min(t, tail_tokens) if tail_tokens else t
        if h + t > max_tokens:
            t = max_tokens - h
        ids = ids[:h] + ids[-t:]
    else:
        # 默认兜底为 tail
        ids = ids[-max_tokens:]
    return tokenizer.decode(ids, skip_special_tokens=False)

def join_code_steps(prev_steps):
    """把历史 completions 列表拼接成串。用轻量分隔符避免无意义的噪声。"""
    return "\n\n# --- prev step ---\n\n".join(prev_steps)

def build_prompt(prev_code, current_code):
    # 你也可以在这里加入 system 指令或可切换的“Reasoning: <level>”占位
    return (
        "You are a precise vulnerability detector for source code.\n"
        "Answer strictly with 'Yes' (vulnerable) or 'No' (not vulnerable).\n\n"
        f"Previous code context:\n{prev_code}\n\n"
        f"Current code:\n{current_code}\n\n"
        "Is the current code vulnerable? Answer with Yes or No."
    )

def safe_model_max_len(tokenizer, fallback=131072):
    m = getattr(tokenizer, "model_max_length", None)
    if m is None or (isinstance(m, int) and (m > 10**9 or m <= 0)) or m == float("inf"):
        return fallback
    return int(m)

def main():
    args = arg_parser()
    model_name = "openai/gpt-oss-20b"
    tokenizer = AutoTokenizer.from_pretrained(model_name)
    llm = LLM(model=model_name, tensor_parallel_size=4)
    sampling_params = SamplingParams(max_tokens=args.max_gen_tokens)

    # 计算输入最大 token：优先用用户给的；否则按模型上限预留生成和少量模板余量
    model_ctx = safe_model_max_len(tokenizer)
    safety_margin = 64  # 模板/特殊符号余量
    if args.max_input_tokens is None:
        max_input_tokens = max(128, model_ctx - args.max_gen_tokens - safety_margin)
    else:
        # 不要超过模型硬上限
        max_input_tokens = min(args.max_input_tokens, model_ctx - args.max_gen_tokens - safety_margin)

    for dataset_name, dataset in datasets.items():
        dataset = dataset.select(range(100))

        prompts_raw = []
        labels_raw = []

        if args.criteria == "steps-level":
            for data in dataset:
                comp = data["completions"]
                lbls = data["labels"]
                # 逐 step 构造
                for i in range(len(comp)):
                    if args.truncate_strategy == "steps-last-k":
                        start = max(0, i - args.steps_k)
                        prev_steps = comp[start:i]
                    else:
                        prev_steps = comp[:i]
                    prev_code = join_code_steps(prev_steps)
                    current_code = comp[i]
                    prompt = build_prompt(prev_code, current_code)
                    prompts_raw.append(prompt)
                    labels_raw.append(lbls[i])
        else:  # sample-level：每个样本 1 条
            for data in dataset:
                comp = data["completions"]
                lbls = data["labels"]
                if args.truncate_strategy == "steps-last-k":
                    prev_steps = comp[max(0, len(comp) - args.steps_k):-1] if len(comp) > 1 else []
                else:
                    prev_steps = comp[:-1] if len(comp) > 1 else []
                prev_code = join_code_steps(prev_steps)
                current_code = comp[-1] if len(comp) > 0 else ""
                prompt = build_prompt(prev_code, current_code)
                prompts_raw.append(prompt)
                labels_raw.append(0 if 0 in lbls else 1)

        # 应用 chat 模板，并按 token 上限截断
        # 注意：不要边遍历边 remove；构造新的列表保持索引一致
        def to_chat(text):  # 可扩展为 system/assistant 角色多消息
            message = [{"role": "user", "content": text}]
            return tokenizer.apply_chat_template(
                message, tokenize=False, add_generation_prompt=True
            )

        texts = []
        labels = []
        for p, lab in zip(prompts_raw, labels_raw):
            templated = to_chat(p)
            # 若超长则截断（对“内容”做截断，然后再套模板，以免截断破坏模板特殊token）
            if count_tokens(templated, tokenizer) > max_input_tokens:
                # 对“用户内容”本身截断
                p_trunc = truncate_text_to_tokens(
                    p, tokenizer, max_input_tokens,
                    strategy=args.truncate_strategy,
                    head_tokens=args.head_tokens,
                    tail_tokens=args.tail_tokens
                )
                templated = to_chat(p_trunc)
                # 如果仍超长（极端情况），再做一次保底截断
                if count_tokens(templated, tokenizer) > max_input_tokens:
                    templated = truncate_text_to_tokens(
                        templated, tokenizer, max_input_tokens,
                        strategy="tail"
                    )
            texts.append(templated)
            labels.append(lab)

        # 三种“推理强度”版本：直接生成三套输入（不要用字符串替换）
        def add_reasoning_level(t, level):
            # 在最前面插入一个轻量前缀，避免破坏模板
            return f"Reasoning: {level}\n\n{t}"

        texts_medium = [add_reasoning_level(t, "medium") for t in texts]
        texts_high   = [add_reasoning_level(t, "high")   for t in texts]
        texts_low    = [add_reasoning_level(t, "low")    for t in texts]

        # 推理
        out_med = llm.generate(texts_medium, sampling_params)
        answer_medium = [o.outputs[0].text for o in out_med]

        out_high = llm.generate(texts_high, sampling_params)
        answer_high = [o.outputs[0].text for o in out_high]

        out_low = llm.generate(texts_low, sampling_params)
        answer_low = [o.outputs[0].text for o in out_low]

        # 评估（保持与 texts / labels 对齐）
        for answers, name in zip([answer_medium, answer_high, answer_low],
                                 ["answer_medium", "answer_high", "answer_low"]):
            TP = FP = TN = FN = invalid = 0
            results = []
            for i in tqdm(range(len(answers))):
                output_text = answers[i]
                pred = extract_label(output_text)
                results.append(output_text)
                if pred == 1 and labels[i] == 1:
                    TN += 1
                elif pred == 0 and labels[i] == 0:
                    TP += 1
                elif pred == 0 and labels[i] == 1:
                    FN += 1
                elif pred == 1 and labels[i] == 0:
                    FP += 1
                elif pred == -1:
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
            denom = (TP + TN + FP + FN)
            if denom == 0:
                print("Accuracy is undefined")
            else:
                print(f"Accuracy: {(TP + TN) / denom}")
            if 2 * TP + FP + FN == 0:
                print("F1-score is undefined")
            else:
                print(f"F1-score: {2 * TP / (2 * TP + FP + FN)}")
            print(f"Total number of results: {len(results)}")
            print(f"Invalid: {invalid / max(1, len(results))}")

            # 每个 answers 都各写一个文件，避免覆盖
            with open(f"baseline_llm_prompting_oss_{name}_{dataset_name}.txt", "w") as f:
                for r in results:
                    f.write(r + "\n")
