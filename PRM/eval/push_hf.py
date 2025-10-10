import transformers
from datasets import load_from_disk

def push_model():
    model_path = '/project/flame/wyu3/PRM/output/stage2/sven_s2_last_10epoch'
    hub_model_id = 'vivi-yu/sven_s2_f1_71'

    model = transformers.AutoModelForTokenClassification.from_pretrained(model_path)
    tokenizer = transformers.AutoTokenizer.from_pretrained(model_path)
    model.push_to_hub(hub_model_id)
    tokenizer.push_to_hub(hub_model_id)
    
def push_dataset():
    dataset_path = '/project/flame/wyu3/PRM/primevul_processed_dataset_unpaired'
    hub_dataset_id = 'vivi-yu/primevul_processed_dataset_unpaired'
    
    dataset = load_from_disk(dataset_path)
    dataset.push_to_hub(hub_dataset_id)

if __name__ == "__main__":
    # push_model()
    push_dataset()