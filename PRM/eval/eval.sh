#  torchrun --nproc_per_node=1 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage1/test2/checkpoint-3000 -b 1
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage2/run1_2/checkpoint-6651 -b 1  -c allsteps > instr_2_6651.log 2>&1 &
# nohup torchrun --nproc_per_node=4 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage2/rep4/checkpoint-4500 -b 1 -c allsteps > coder_rep4_1_4k_allsteps.log 2>&1 &
# nohup torchrun --nproc_per_node=4 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage1/test_coder_replicate_4/checkpoint-1000 -b 1  > coder_rep_4.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46003 code_bigvul_bench.py -m  /project/flame/wyu3/PRM/output/stage2/rep4/checkpoint-4500 -b 1 > bigvul_rep4_s2_4.5k.log 2>&1 &
export PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True
## bigvul
# nohup torchrun --nproc_per_node=4 --master_port 46003 code_bigvul_bench.py -m Qwen/Qwen2.5-Math-PRM-7B  -b 1 -d bigvul_dedup > bigvul_baseline_qwen_dedup.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_bigvul_bench.py  -m  /project/flame/wyu3/PRM/output/stage2/bigvul_s2_3epoch_add_ckp1500/checkpoint-1000/ -b 1 -d bigvul_dedup  > bigvul_s2_ckp1000_add_rep2.log 2>&1 &

# sven
# nohup torchrun --nproc_per_node=4 --master_port 46003 code_sven_bench.py -m /project/flame/wyu3/PRM/output/stage1/coder_s1_sven_30epoch -d sven_val -t 0.1 > sven_s1_30epoch_sven_val_t0.1.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_precise_bench.py -m  /project/flame/wyu3/PRM/output/stage1/test_coder_rep_4 -b 1 > precise_rep_4_validation.log 2>&1 &

# nohup torchrun --nproc_per_node=4 --master_port 46001 code_primevul_bench.py -m /project/flame/wyu3/PRM/output/stage0/coder_s0_primevul_5epoch_unpaired/checkpoint-4000/  -b 1  -d primevul_test_unpaired > primevul_stage0_unpaired_ckp4k.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46002 code_primevul_bench.py -m  Qwen/Qwen2.5-Math-PRM-7B  -b 1  -d primevul_test_paired > primevul_baseline_qwen_paired.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46001 code_primevul_bench.py -m /project/flame/wyu3/PRM/output/stage1/coder_s1_primevul_froms0_lastonly_10epoch_paired  -b 1  -d primevul_test_unpaired > primevul_coder_s1_froms0_lastonly_10epoch_unpaired.log 2>&1 &

