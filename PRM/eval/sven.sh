for prm_name in Qwen/Qwen2.5-Math-PRM-7B vivi-yu/sven_s2_f1_71 vivi-yu/precise_s1 vivi-yu/prime_s2_ckp500_from_s0_all; do
    for c1 in ave binary min last_position; do
        for c2 in softmax plain advantage; do
            name="${prm_name}_${c1}_${c2}_sven"
            python ranking_code_bon_sven_new.py --prm_name "$prm_name" --criterion1 "$c1" --criterion2 "$c2" --name "$name"
        done
    done
done
