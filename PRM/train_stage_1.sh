# export OMP_NUM_THREADS=4  # 每个GPU进程2个线程
# export MKL_NUM_THREADS=4

GPUS_PER_NODE=1
NNODES=1
NODE_RANK=0
MASTER_ADDR=localhost
MASTER_PORT=46001

DISTRIBUTED_ARGS="
    --nproc_per_node $GPUS_PER_NODE \
    --nnodes $NNODES \
    --node_rank $NODE_RANK \
    --master_addr $MASTER_ADDR \
    --master_port $MASTER_PORT
"

torchrun $DISTRIBUTED_ARGS finetune.py \
    --model_name_or_path /project/flame/wyu3/PRM/output/stage2/rep4/checkpoint-4500 \
    --train_data_path "/project/flame/wyu3/PRM/sven_processed_dataset" \
    --fix_llm True \
    --num_train_epochs 9 \
    --learning_rate 1e-4 \
    --run_name coder_last_s1_sven \
    --output_dir /project/flame/wyu3/PRM/output/stage1/coder_last_s1_sven \
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
    --deepspeed ds_config_zero2.json