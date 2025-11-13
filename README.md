<div align="center">


## 🔧 Quick Start

We implement our algorithm on two frameworks, OpenRLHF and verl, in 2 different branches respectively. If you are new to our project, we recommend using verl version.

### Installation

#### 1. OpenRLHF version

Please follow [OpenRLHF's guidance](https://github.com/OpenRLHF/OpenRLHF/tree/main?tab=readme-ov-file#installation) to configure required environments. Then run `pip install -r requirements.txt`.

#### 2. verl version

Please refer to the [official installation guidance](https://verl.readthedocs.io/en/latest/start/install.html#install-from-custom-environment) of verl.

### Training of PRM

We train the PRM in 2 stages using [TRL](https://github.com/huggingface/trl) and a [preprocessed PRM800K dataset](https://huggingface.co/datasets/HuggingFaceH4/prm800k-trl-dedup). In the first stage, we freeze the LLM and only train the last score layer (MLP) with 1e-4 learning rate rate for 3 epochs. In the second stage, we unfreeze the LLM and fine-tune all parameters with 1e-6 learning rate for 1 epoch. The resultant PRM is released through [HuggingFace](https://huggingface.co/jinachris/Qwen2.5-Math-7B-PRM800K).

```bash
cd PRM
# stage 1
bash train_stage_1.sh
# stage 2
bash train_stage_2.sh
```

### Eval of PRM

#### 1. eval all the benchmark using 

```bash
bash PRM/eval/eval_all.sh   model_dir
```


#### 2. eval single benchmark
```bash
bash PRM/eval/eval.sh  
```

## acknowledgement
