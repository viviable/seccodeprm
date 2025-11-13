nohup bash -c "CUDA_VISIBLE_DEVICES=0 python moe_code_bench.py -d precisebugs_test" \
    > moe_precise.log 2>&1 &
    
nohup bash -c "CUDA_VISIBLE_DEVICES=1 python moe_code_bench.py -d primevul_test" \
    > moe_primevul.log 2>&1 &

nohup bash -c "CUDA_VISIBLE_DEVICES=2 python moe_code_bench.py -d reposvul_test" \
    > moe_reposvul.log 2>&1 &

nohup bash -c "CUDA_VISIBLE_DEVICES=3 python moe_code_bench.py -d sven_val" \
    > moe_sven.log 2>&1 &
