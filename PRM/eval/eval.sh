#  torchrun --nproc_per_node=1 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage1/test2/checkpoint-3000 -b 1
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage2/run1_2/checkpoint-6651 -b 1  -c allsteps > instr_2_6651.log 2>&1 &
# nohup torchrun --nproc_per_node=4 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage2/rep4/checkpoint-4500 -b 1 -c allsteps > coder_rep4_1_4k_allsteps.log 2>&1 &
# nohup torchrun --nproc_per_node=4 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage1/test_coder_replicate_4/checkpoint-1000 -b 1  > coder_rep_4.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46003 code_bigvul_bench.py -m  /project/flame/wyu3/PRM/output/stage2/rep4/checkpoint-4500 -b 1 > bigvul_rep4_s2_4.5k.log 2>&1 &
export PYTORCH_CUDA_ALLOC_CONF=expandable_segments:True
## bigvul
# nohup torchrun --nproc_per_node=4 --master_port 46003 code_bigvul_bench.py -m Qwen/Qwen2.5-Math-PRM-7B  -b 1 -d bigvul_dedup > bigvul_baseline_qwen_dedup.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_bigvul_bench.py  -m  /project/flame/wyu3/PRM/output/stage0/coder_s0_bigvul_1epoch/checkpoint-1248 -b 1 -d bigvul_dedup  -c allsteps > bigvul_s0_ckp1248_allsteps.log 2>&1 &

# sven
# nohup torchrun --nproc_per_node=4 --master_port 46001 code_sven_bench.py -m /project/flame/wyu3/PRM/output/stage1/coder_last_s1_sven_30epoch/checkpoint-360 -d sven_val -t 0.1 > sven_s1_30epoch_sven_val_t0.1.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46001 code_sven_bench.py -m Qwen/Qwen2.5-Math-PRM-72B -d sven_val -t 0.1 -c allsteps > sven_baseline_qwenprm_72b.log 2>&1 &

#precise bugs
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_precise_bench.py -m  /project/flame/wyu3/PRM/output/stage2/precisebugs_s2_1epoch_s1_918 -b 1  -c allsteps > precise_s2_1epoch_1632_allsteps.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_precise_bench.py -m  Qwen/Qwen2.5-Math-PRM-7B  -b 1 > precise_baseline_qwen2.5_7b.log 2>&1 &

#primevul
# nohup torchrun --nproc_per_node=4 --master_port 46003 code_primevul_bench.py -m /project/flame/wyu3/PRM/output/stage2/primevul_s2_3epoch_s1_s2/checkpoint-1827/  -b 1  -d primevul_test_paired  -c allsteps > primevul_stage2_paired_ckp1827_allsteps.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46002 code_primevul_bench.py -m  Qwen/Qwen2.5-Math-PRM-7B  -b 1  -d primevul_test_paired > primevul_baseline_qwen_paired.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46001 code_primevul_bench.py -m /project/flame/wyu3/PRM/output/stage1/coder_s1_primevul_froms0_lastonly_10epoch_paired  -b 1  -d primevul_test_unpaired > primevul_coder_s1_froms0_lastonly_10epoch_unpaired.log 2>&1 &

# transfer
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_repos_bench.py -m /project/flame/wyu3/PRM/output/stage1/coder_last_s1_sven_90epoch/checkpoint-1080 -d reposvul_test -t 0.1 > transfer_sven_s1_ckp1080_repos_paired_t0.1.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_repos_bench.py -m /project/flame/wyu3/PRM/output/stage1/coder_last_s1_sven_90epoch/checkpoint-1080 -d reposvul_test -t 0.1 > transfer_sven_s1_ckp1080_repos_paired_t0.1.log 2>&1 &

# reposvul
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_repos_bench.py -m Qwen/Qwen2.5-Math-PRM-7B -b 1 -d reposvul_test -t 0.1 > reposvul_baseline_qwen_7b.log 2>&1 &
nohup torchrun --nproc_per_node=4 --master_port 46005 code_repos_bench.py -m /project/flame/wyu3/PRM/output/stage1/coder_s1_reposvul_3epoch_merged -b 1 -d reposvul_test  -t 0.1 > reposvul_s1_reposvul_3epoch_merged_t0.1.log 2>&1 &

## moe
# nohup torchrun --nproc_per_node=4 --master_port 46005 moe_code_bench.py  -d precisebugs_test -t 0.1 > moe_precisebugs.log 2>&1 &