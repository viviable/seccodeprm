#  torchrun --nproc_per_node=1 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage1/test2/checkpoint-3000 -b 1
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage2/run1_2/checkpoint-6651 -b 1  -c allsteps > instr_2_6651.log 2>&1 &
# nohup torchrun --nproc_per_node=4 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage2/rep4/checkpoint-4500 -b 1 -c allsteps > coder_rep4_1_4k_allsteps.log 2>&1 &
# nohup torchrun --nproc_per_node=4 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage1/test_coder_replicate_4/checkpoint-1000 -b 1  > coder_rep_4.log 2>&1 &


# nohup torchrun --nproc_per_node=1 --master_port 46004 code_bigvul_bench.py -m  /project/flame/wyu3/PRM/output/stage1/coder_last_s1_sven -b 1 -w 1 > sven_s1_last_test.log 2>&1 &
nohup torchrun --nproc_per_node=1 --master_port 46004 code_sven_bench.py -m  /project/flame/wyu3/PRM/output/stage1/coder_last_s1_sven -d sven_val > sven_s1_last_test_sven_val.log 2>&1 &
