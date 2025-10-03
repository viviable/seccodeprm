import transformers

model_path = '/project/flame/wyu3/PRM/output/stage2/primevul_s2_3epoch_s0all/checkpoint-1000/'
hub_model_id = 'vivi-yu/prime_s2_ckp1000_from_s0_all'

model = transformers.AutoModelForTokenClassification.from_pretrained(model_path)
tokenizer = transformers.AutoTokenizer.from_pretrained(model_path)
model.push_to_hub(hub_model_id)
tokenizer.push_to_hub(hub_model_id)

