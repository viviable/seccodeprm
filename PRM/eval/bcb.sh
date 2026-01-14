bigcodebench.evaluate \
  --model meta-llama/Meta-Llama-3.1-8B-Instruct \
  --execution gradio  \
  --split instruct \
  --subset full \
  --backend vllm \
  -- /project/flame/wyu3/PRM/full/complete/claude-3-5-sonnet-20241022--bigcodebench-complete--anthropic-0-1-sanitized_calibrated.jsonl