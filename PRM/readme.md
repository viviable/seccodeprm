# Setup and Training Instructions

## 1. Generate the Data

First, run the data collection script to generate the processed dataset:

```bash
python ./collect_data.py
```

The processed data will be saved to `./bigvul_processed_dataset`.

## 2. Run Stage 1 Training

### Modify Training Script

Before running the training, you need to update `train_stage_1.sh`:

1. Change the `--train_data_path` parameter:
   ```bash
   # FROM:
   --train_data_path "/project/flame/wyu3/PRM/bigvul_processed_dataset"
   
   # TO:
   --train_data_path "./bigvul_processed_dataset"
   ```

2. Change the `--output_dir` to your desired directory:
   ```bash
   --output_dir <your_dir>
   ```

### Run Training

```bash
bash train_stage_1.sh
```

The checkpoint will be saved to `<your_dir>`.

## 3. Evaluate the Checkpoint

Navigate to the evaluation directory and run evaluation:

```bash
cd eval/
```

### Modify Evaluation Script

In `eval.sh`, change the `-m` parameter to point to your checkpoint:

```bash
-m <your_dir>/checkpoint-XX
```

### Run Evaluation

```bash
bash eval.sh
```

## Notes

- Replace `<your_dir>` with your actual output directory path
- Replace `checkpoint-XX` with the actual checkpoint number generated during training