# SecCodePRM

SecCodePRM is a codebase for training and evaluating PRMs (Process/Preference/Policy Reward Models; naming per your paper/project) in secure coding scenarios. The repository is organized by data processing, training, and evaluation workflows.

## Repository Layout

- `data/`: data collection, cleaning, and dataset utilities
- `train/`: training scripts and configs
- `eval/`: evaluation scripts and configs
- `requirements.txt`: dependency list
- `setup.py` / `pyproject.toml`: packaging/build configuration
- `nccl.conf`: distributed training config (if needed)

## Environment

Recommended Python 3.8+.

```bash
pip install -r requirements.txt
```

If you only need a subset of functionality, you can install a reduced set of dependencies.

## Data Preparation

Example data collection commands (see `data/collect_data.py` for all options):

```bash
# BigVul
python data/collect_data.py bigvul --output ./bigvul_processed_dataset

# PreciseBugs
python data/collect_data.py precise --dir /project/flame/wyu3/PRM/PreciseBugs/CVEs --cwe CWE-639 --output ./precisebugs_processed_dataset

# PrimeVul (paired)
python data/collect_data.py primevul --paired --base-dir /project/flame/wyu3/PRM/PrimeVul_v0.1 --output /project/flame/wyu3/PRM/primevul_processed_dataset_paired

# ReposVul
python data/collect_data.py reposvul --base-dir /project/flame/wyu3/PRM/ReposVul --languages c cpp java python --output /project/flame/wyu3/PRM/reposvul_processed_dataset

# SVEN
python data/collect_data.py sven --base-dir /project/flame/wyu3/PRM/sven_git/sven/data_eval/trained --output /project/flame/wyu3/PRM/sven_processed_dataset

# Concatenate multiple datasets
python data/collect_data.py all --output /project/flame/wyu3/PRM/all_processed_dataset_31340
```

Check each script's arguments and default paths before running.

## Training

Multi-stage training scripts live in `train/`.

```bash
bash train/train_stage_1.sh
bash train/train_stage_2.sh
```

Update data paths and output directories inside the scripts as needed.

## Evaluation

Evaluation scripts live in `eval/`. Example (BoN ranking):

```bash
# CWEval
python eval/ranking_code_bon.py cweval --generated_dir <dir> --prm_name <model> --name run

# LiveCodeBench
python eval/ranking_code_bon.py livecodebench --input_path <json> --prm_name <model> --name run

# SVEN
python eval/ranking_code_bon.py sven --output_dir <dir> --prm_name <model> --name run
```

Other benchmarks and utilities can be found under `eval/`. 

## Cite


## License

Add a LICENSE file before open sourcing and update this section accordingly.
