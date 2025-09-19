#  torchrun --nproc_per_node=1 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage1/test2/checkpoint-3000 -b 1
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage2/run1_2/checkpoint-6651 -b 1  -c allsteps > instr_2_6651.log 2>&1 &
# nohup torchrun --nproc_per_node=4 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage2/rep4/checkpoint-4500 -b 1 -c allsteps > coder_rep4_1_4k_allsteps.log 2>&1 &
# nohup torchrun --nproc_per_node=4 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage1/test_coder_replicate_4/checkpoint-1000 -b 1  > coder_rep_4.log 2>&1 &


nohup torchrun --nproc_per_node=4 --master_port 46004 code_bigvul_bench.py -m  /project/flame/wyu3/PRM/output/stage2/rep4/checkpoint-4000 -b 1 > bigvul_rep_4_s2_ckp4k.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46003 code_sven_bench.py -m  /project/flame/wyu3/PRM/output/stage1/coder_last_s1_sven_90epoch -d sven_val -t 0.1 > sven_s1_90epoch_test_t0.1.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_precise_bench.py -m  /project/flame/wyu3/PRM/output/stage1/test_coder_rep_4 -b 1 > precise_rep_4_validation.log 2>&1 &
# nohup torchrun --nproc_per_node=4 --master_port 46003 code_primevul_bench.py -m  /project/flame/wyu3/PRM/output/stage1/coder_s1_primevul_10epoch -b 1 > primevul_1k_s1_10epoch.log 2>&1 &
