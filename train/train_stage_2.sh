GPUS_PER_NODE=$(nvidia-smi | grep 80GB | wc -l)
NNODES=1
NODE_RANK=0
MASTER_ADDR=localhost
MASTER_PORT=46008

DISTRIBUTED_ARGS="
    --nproc_per_node $GPUS_PER_NODE \
    --nnodes $NNODES \
    --node_rank $NODE_RANK \
    --master_addr $MASTER_ADDR \
    --master_port $MASTER_PORT
"

torchrun $DISTRIBUTED_ARGS finetune_wckp.py \
    --model_name_or_path your_stage1_output_dir \
    --train_data_path your_train_data_path \
    --fix_llm False \
    --num_train_epochs 1 \
    --learning_rate 5e-7 \
    --run_name your_run_name \
    --output_dir your_output_dir \
    --bf16 True \
    --per_device_train_batch_size 6 \
    --per_device_eval_batch_size 1 \
    --gradient_accumulation_steps 1 \
    --evaluation_strategy "no" \
    --save_strategy "steps" \
    --save_steps 500 \
    --save_total_limit 10 \
    --weight_decay 0.1 \
    --adam_beta2 0.95 \
    --warmup_ratio 0.0 \
    --logging_steps 1 \
    --report_to "wandb" \
    --gradient_checkpointing True \
    --deepspeed ds_config_zero2.json \
    --enable_load_ckpt True 
