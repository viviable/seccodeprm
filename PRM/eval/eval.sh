#  torchrun --nproc_per_node=1 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage1/test2/checkpoint-3000 -b 1
# nohup torchrun --nproc_per_node=4 --master_port 46005 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage2/run1_2/checkpoint-6651 -b 1  -c allsteps > instr_2_6651.log 2>&1 &
# nohup torchrun --nproc_per_node=4 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage2/rep4/checkpoint-4500 -b 1 -c allsteps > coder_rep4_1_4k_allsteps.log 2>&1 &
# nohup torchrun --nproc_per_node=4 code_binary_process_bench.py -m /project/flame/wyu3/PRM/output/stage1/test_coder_replicate_4/checkpoint-1000 -b 1  > coder_rep_4.log 2>&1 &


nohup torchrun --nproc_per_node=1 --master_port 46004 code_binary_process_bench.py -m  /project/flame/wyu3/PRM/output/stage1/test_coder_one_zero_rep4/checkpoint-1000 -b 1 > onezero_s1_rep4_1000_test.log 2>&1 &
