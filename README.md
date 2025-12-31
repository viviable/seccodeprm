<div align="left">


## 🔧 Quick Start

We implement our algorithm on two frameworks, OpenRLHF and verl, in 2 different branches respectively. If you are new to our project, we recommend using verl version.

### Installation

#### 1. OpenRLHF version

Please follow [OpenRLHF's guidance](https://github.com/OpenRLHF/OpenRLHF/tree/main?tab=readme-ov-file#installation) to configure required environments. Then run `pip install -r requirements.txt`.

#### 2. verl version

Please refer to the [official installation guidance](https://verl.readthedocs.io/en/latest/start/install.html#install-from-custom-environment) of verl.

### Training of PRM

We train the PRM in 2 stages using [TRL](https://github.com/huggingface/trl) and a [preprocessed vul dataset](https://huggingface.co/vivi-yu). In the first stage, we freeze the LLM and only train the last score layer (MLP) with 1e-4 learning rate rate for 3 epochs. In the second stage, we unfreeze the LLM and fine-tune all parameters with 1e-6 learning rate for 1 epoch. 
```bash
cd PRM
# stage 1
bash ./PRM/train_stage_1_all_wckp.sh
# stage 2
bash ./PRM/train_stage_2.sh
```

### Eval of PRM
Eval tips: temperature=0.1 works better for every benchmark.
#### 1. eval all the benchmark using 

```bash
bash ./PRM/eval/eval_all.sh   model_dir
```


#### 2. eval single benchmark
```bash
bash ./PRM/eval/eval.sh  
```

#### 3. eval ensemble model benchmark
```bash
bash ./PRM/eval/moe.sh  
```

#### 3. eval code generation benchmark
```python
python ./PRM/eval/ranking_code_bon_sven.py
```
For CWEval, please refer to https://github.com/Co1lin/CWEval and note that they operate in docker.
refer to https://github.com/Co1lin/CWEval?tab=readme-ov-file#2-generate-llm-responses for generating the LLM responses, set -n as 10, 30, or 50, then use the following for BoN to get safer candidates.
```python
python ./PRM/eval/ranking_code_bon_cweval.py --gen_dir <your-gen-dir>
```

For HumanEval, please refer to https://github.com/bigcode-project/bigcodebench for installation.


## Acknowledgement

