# BigVul
python data/collect_data.py bigvul --dir your_bigvul_dir --output your_bigvul_processed_dataset

# PreciseBugs
python data/collect_data.py precise --dir your_precisebugs_dir --cwe CWE-639 --output your_precisebugs_processed_dataset

# PrimeVul (paired)
python data/collect_data.py primevul --paired --base-dir your_primevul_dir --output your_primevul_processed_dataset_paired

# ReposVul
python data/collect_data.py reposvul --base-dir your_reposvul_dir --languages c cpp java python --output your_reposvul_processed_dataset

# SVEN
python data/collect_data.py sven --base-dir your_sven_dir --output your_sven_processed_dataset

# 合并多个数据集
python data/collect_data.py all --output your_all_processed_dataset
