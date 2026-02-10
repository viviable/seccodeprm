prm_name="Qwen/Qwen2.5-Math-PRM-7B"
c1="ave"
c2="softmax"
name="${prm_name}_${c1}_${c2}_livecodebench"
python ranking_code_bon_livecodebench.py --prm_name "$prm_name" --criterion1 "$c1" --criterion2 "$c2" --name "$name" --input_path "your_livecodebench_generation.json"

