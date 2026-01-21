import transformers
from datasets import load_from_disk

def push_model():
    model_path = '/project/flame/wyu3/PRM/output/stage1/precise_s1_coder'
    hub_model_id = 'vivi-yu/precise_s1'

    model = transformers.AutoModelForTokenClassification.from_pretrained(model_path)
    tokenizer = transformers.AutoTokenizer.from_pretrained(model_path)
    model.push_to_hub(hub_model_id)
    tokenizer.push_to_hub(hub_model_id)
    
def unify_dataset(dataset):
    from datasets import Features, Value, Sequence
    from datasets import Dataset, DatasetDict
    
    train_data = dataset['train'].to_dict()
    test_data = dataset['test'].to_dict()

    # 手动定义统一的特征
    features = Features({
        'prompt': Value('string'),
        'completions': Sequence(Value('string')),
        'labels': Sequence(Value('int64')),
        'source': Value('string'),
        'index': Value('int64'),
        'other_info': {
            # ... 其他字段 ...
            'other': {
                'cve': {
                    # 将可能为 null 的字段统一定义为 string
                    'cisaActionDue': Value('string'),
                    'cisaExploitAdd': Value('string'),
                    'cisaRequiredAction': Value('string'),
                    'cisaVulnerabilityName': Value('string'),
                    
                    # 将 vendorComments 统一定义
                    'vendorComments': Sequence({
                        'comment': Value('string'),
                        'lastModified': Value('string'),
                        'organization': Value('string')
                    }),
                    
                    # ... 其他字段 ...
                }
            }
        }
    })
    # 加载数据时指定特征
    dataset = DatasetDict({
        'train': Dataset.from_dict(train_data, features=features),
        'test': Dataset.from_dict(test_data, features=features)
    })
    return dataset

def push_dataset():
    dataset_path = '/project/flame/wyu3/PRM/all_processed_dataset_31340'
    hub_dataset_id = 'vivi-yu/all_processed_31340'
    
    
    dataset = load_from_disk(dataset_path)
    # dataset = unify_dataset(dataset)
    # dataset = dataset.remove_columns(['other_info'])
    dataset.push_to_hub(hub_dataset_id)

def save_tokenized_dataset(tokenizer_path, dataset_path, output_path):
    # 创建一个预处理脚本 preprocess_data.py

    def tokenize_function(examples, tokenizer):
        # 根据你的数据格式调整
        return tokenizer(
            examples['text'],  # 或其他字段名
            max_length=128000,
            truncation=True,
            padding=False,  # 不在这里padding，让collator处理
        )

    # 加载原始数据和tokenizer
    tokenizer = transformers.AutoTokenizer.from_pretrained("Qwen/Qwen2-0.5B")
    dataset = load_from_disk("trl-lib/math_shepherd")

    # 批量tokenize
    tokenized_dataset = dataset.map(
        lambda x: tokenize_function(x, tokenizer),
        batched=True,
        num_proc=4,  # 多进程加速
        desc="Tokenizing dataset"
    )

    # 保存tokenized数据集
    tokenized_dataset.save_to_disk("trl-lib/math_shepherd_tokenized")

if __name__ == "__main__":
    # push_model()
    push_dataset()