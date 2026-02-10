bigcodebench.evaluate \
  --model meta-llama/Meta-Llama-3.1-8B-Instruct \
  --execution gradio  \
  --split instruct \
  --subset full \
  --backend vllm \
  -- your_bcb_generation.jsonl