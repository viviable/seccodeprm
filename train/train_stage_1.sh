GPUS_PER_NODE=$(nvidia-smi | grep 80GB | wc -l)
NNODES=1
NODE_RANK=0
MASTER_ADDR=localhost
MASTER_PORT=46006

DISTRIBUTED_ARGS="
    --nproc_per_node $GPUS_PER_NODE \
    --nnodes $NNODES \
    --node_rank $NODE_RANK \
    --master_addr $MASTER_ADDR \
    --master_port $MASTER_PORT
"

torchrun $DISTRIBUTED_ARGS finetune_wckp.py \
    --model_name_or_path Qwen/Qwen2.5-Coder-7B-Instruct \
    --train_data_path "your_train_data_path" \
    --fix_llm True \
    --num_train_epochs 3 \
    --learning_rate 1e-4 \
    --run_name your_run_name \
    --output_dir your_output_dir \
    --bf16 True \
    --per_device_train_batch_size 32 \
    --per_device_eval_batch_size 1 \
    --gradient_accumulation_steps 1 \
    --evaluation_strategy "no" \
    --save_strategy "steps" \
    --save_steps 1000 \
    --save_total_limit 10 \
    --weight_decay 0.1 \
    --adam_beta2 0.95 \
    --warmup_ratio 0.0 \
    --logging_steps 1 \
    --report_to "wandb" \
    --gradient_checkpointing True \
    --deepspeed ds_config_zero2.json \
    --ddp_timeout 3600 \
    --enable_load_ckpt False \
    --soft False
