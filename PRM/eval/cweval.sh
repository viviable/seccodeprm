model_name='qwen3_30B'
for prm_name in Qwen/Qwen2.5-Math-PRM-7B vivi-yu/sven_s2_f1_71 vivi-yu/precise_s1 vivi-yu/prime_s2_ckp500_from_s0_all; do
    for c1 in ave binary min last_position; do
        for c2 in softmax plain advantage; do
            name="${model_name}_${prm_name}_${c1}_${c2}_cweval"
            python ranking_code_bon_cweval.py --prm_name "$prm_name" --criterion1 "$c1" --criterion2 "$c2" --name "$name" --generated_dir /project/flame/wyu3/PRM/bon/eval_qwen3_30b/
        done
    done
done
