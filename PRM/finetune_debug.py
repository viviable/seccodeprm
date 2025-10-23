import os
from dataclasses import dataclass, field
from typing import Dict, Optional

import transformers
from datasets import load_dataset, load_from_disk, concatenate_datasets
from torch import distributed as dist
from trl import PRMConfig, PRMTrainer
import torch
import re

import random
import numpy as np

@dataclass
class ModelArguments:
    model_name_or_path: Optional[str] = field(default="Qwen/Qwen2-0.5B")


@dataclass
class DataArguments:
    train_data_path: str = field(default="trl-lib/math_shepherd")
    eval_data_path: str = field(default=None)
    lazy_preprocess: bool = False
    only_vul: bool = False
    resample: bool = False


@dataclass
class TrainingArguments(PRMConfig):
    cache_dir: Optional[str] = field(default=None)
    max_length: int = field(default=128000)
    max_completion_length: int = field(default=8000)
    fix_llm: bool = field(default=False)


def safe_save_model_for_hf_trainer(
        trainer: transformers.Trainer, 
        output_dir: str,
    ):
    """Collects the state dict and dump to disk."""
    state_dict = trainer.model.state_dict()

    if trainer.args.should_save and trainer.args.local_rank == 0:
        trainer._save(output_dir, state_dict=state_dict)
        


def make_supervised_data_module(data_args) -> Dict:
    """Make dataset and collator for supervised fine-tuning."""
    assert data_args.train_data_path is not None
    # if 'bigvul' in data_args.train_data_path or 'precise' in data_args.train_data_path or 'sven' in data_args.train_data_path or 'primevul' in data_args.train_data_path:
    train_dataset = load_from_disk(data_args.train_data_path)['train']
    eval_dataset = load_from_disk(data_args.train_data_path)['test']
    resample = False
    if resample:
        # Step 1: Filter data with label 0
        minority_data = train_dataset.filter(lambda x: 0 in x['labels'])
        # Step 2: Replicate the minority data
        replicate_ratio = 2
        replicated_datasets = [minority_data] * replicate_ratio
        # Step 3: Concatenate original dataset with replicated minority data
        balanced_dataset = concatenate_datasets([train_dataset] + replicated_datasets)
        balanced_dataset = balanced_dataset.shuffle(seed=42)
    else:
        balanced_dataset = train_dataset
        
    only_vul = False
    if only_vul:
        balanced_dataset = balanced_dataset.filter(lambda x: 0 in x['labels'])
    
    # else:
    #     train_dataset = load_dataset(data_args.train_data_path, split="train")
    #     eval_dataset = load_dataset(data_args.train_data_path, split="test")

    return dict(
        train_dataset=balanced_dataset, 
        eval_dataset=eval_dataset, 
    )

def get_rng_state():
    state = {
        "py_random": random.getstate(),
        "np_random": np.random.get_state(),
        "torch_cpu": torch.get_rng_state(),
    }
    if torch.cuda.is_available():
        state["torch_cuda_all"] = torch.cuda.get_rng_state_all()
    return state

def set_rng_state(state):
    random.setstate(state["py_random"])
    np.random.set_state(state["np_random"])
    torch.set_rng_state(state["torch_cpu"])
    if torch.cuda.is_available() and "torch_cuda_all" in state:
        torch.cuda.set_rng_state_all(state["torch_cuda_all"])
        
def load(path, model, optimizer,
             scheduler=None, scaler=None, map_location="cpu"):
        ckpt = torch.load(path, map_location=map_location)
        (model.module if hasattr(model, "module") else model).load_state_dict(ckpt["model"])
        optimizer.load_state_dict(ckpt["optimizer"])
        if scheduler is not None and "scheduler" in ckpt:
            scheduler.load_state_dict(ckpt["scheduler"])
        if scaler is not None and "scaler" in ckpt:
            scaler.load_state_dict(ckpt["scaler"])
        set_rng_state(ckpt["rng"])
        epoch        = ckpt["epoch"]
        global_step  = ckpt["global_step"]
        step_in_epoch= ckpt.get("step_in_epoch", 0)
        extra        = ckpt.get("extra", {})
        return epoch, global_step, step_in_epoch, extra
    
def train():
    os.environ["WANDB_PROJECT"]="PRM_Math_Shepherd"
    
    parser = transformers.HfArgumentParser(
        (ModelArguments, DataArguments, TrainingArguments)
    )
    
    (
        model_args,
        data_args,
        training_args,
        _
    ) = parser.parse_args_into_dataclasses(return_remaining_strings=True)

    # Set random seeds
    torch.manual_seed(training_args.seed)
    np.random.seed(training_args.seed)
    random.seed(training_args.seed)

    # Load model and tokenizer
    model = transformers.AutoModelForTokenClassification.from_pretrained(
        model_args.model_name_or_path,
        cache_dir=training_args.cache_dir,
        trust_remote_code=True,
        use_cache = False,
        torch_dtype=torch.bfloat16,  # 🔥 关键！直接用bf16加载
        low_cpu_mem_usage=True,       # 🔥 降低加载时内存峰值
        device_map=None,              # DeepSpeed会自己处理设备分配
    )
    

    # freeze llm except last layer if needed
    if training_args.fix_llm:
        model.model.requires_grad_(False)
                
    tokenizer = transformers.AutoTokenizer.from_pretrained(
        model_args.model_name_or_path,
        cache_dir=training_args.cache_dir,
        use_fast=False,
        trust_remote_code=True,
    )
    # Load data
    data_module = make_supervised_data_module(data_args=data_args)

    # Start trainner
    # print('training_args', training_args)
    
    trainer = PRMTrainer(
        model=model, 
        processing_class=tokenizer, 
        args=training_args, 
        **data_module
    )
    print('training_args.resume_from_checkpoint', training_args.resume_from_checkpoint)  # string
    if training_args.resume_from_checkpoint != 'False' and os.path.exists(training_args.output_dir):
        print('resume from checkpoint')
        ckpts = [d for d in os.listdir(training_args.output_dir) if d.startswith("checkpoint-")]
        if ckpts:
            latest = max(ckpts, key=lambda x: int(re.findall(r"\d+", x)[0]))
            latest_path = os.path.join(training_args.output_dir, latest)
            print('loading checkpoint from', latest_path)
            print(f'Before training - global_step: {trainer.state.global_step}')
            print(f'Before training - save_steps: {training_args.save_steps}')
            trainer.train(resume_from_checkpoint=latest_path)
            # trainer.train(resume_from_checkpoint=True)
            
    else:
        print('no checkpoint found, training from scratch')
        trainer.train()
    trainer.save_state()
    #['args', 'compute_loss_func', 'hp_name', 'deepspeed', 'is_in_train', 'model', 'accelerator', 'gather_function', 'is_deepspeed_enabled', 'is_fsdp_enabled', 'is_tp_enabled', '_memory_tracker', 'model_init', 'is_model_parallel', 'is_fsdp_xla_enabled', 'place_model_on_device', 'data_collator', 'train_dataset', 'eval_dataset', 'processing_class', 'model_wrapped', 'model_accepts_loss_kwargs', 'neftune_noise_alpha', 'compute_metrics', 'preprocess_logits_for_metrics', 'optimizer', 'lr_scheduler', 'optimizer_cls_and_kwargs', 'callback_handler', '_loggers_initialized', 'hub_model_id', '_signature_columns', 'use_apex', 'use_cpu_amp', 'label_smoother', 'control', 'state', 'current_flos', 'hp_search_backend', 'label_names', 'can_return_loss', '_train_batch_size', '_created_lr_scheduler', 'is_fsdp_xla_v2_enabled', 'is_fsdp_xla_v1_enabled', '__module__', '__doc__', '_tag_names', '__init__', 'tokenize_row', '_save_checkpoint', 'create_model_card', '_get_learning_rate', 'log_metrics', 'metrics_format', 'save_metrics', 'save_state', 'tokenizer', '_activate_neftune', '_deactivate_neftune', 'add_callback', 'pop_callback', 'remove_callback', '_move_model_to_device', '_align_special_tokens', '_set_signature_columns_if_needed', '_remove_unused_columns', '_get_collator_with_removed_columns', '_get_train_sampler', '_get_dataloader', 'get_train_dataloader', '_get_eval_sampler', 'get_eval_dataloader', 'get_test_dataloader', 'create_optimizer_and_scheduler', 'get_decay_parameter_names', 'create_optimizer', 'get_num_trainable_parameters', 'get_learning_rates', 'get_optimizer_group', 'get_optimizer_cls_and_kwargs', 'create_scheduler', 'num_examples', 'num_tokens', '_hp_search_setup', '_report_to_hp_search', '_tune_save_checkpoint', 'call_model_init', 'torch_jit_model_eval', 'compare_trainer_and_checkpoint_args', '_wrap_model', 'train', 'get_tp_size', 'get_total_train_batch_size', '_inner_training_loop', '_get_output_dir', '_load_from_checkpoint', '_load_best_model', '_issue_warnings_after_load', '_evaluate', '_maybe_log_save_evaluate', '_load_rng_state', '_determine_best_metric', '_save_rng_state', '_save_optimizer_and_scheduler', '_load_optimizer_and_scheduler', '_save_scaler', '_load_scaler', '_load_callback_state', 'hyperparameter_search', 'log', '_prepare_input', '_prepare_inputs', '_is_attention_mask_causal', '_prepare_context_parallel_inputs', 'compute_loss_context_manager', 'autocast_smart_context_manager', 'training_step', 'compute_loss', 'is_local_process_zero', 'is_world_process_zero', 'save_model', '_save_tpu', '_save', 'store_flos', '_sorted_checkpoints', '_rotate_checkpoints', 'evaluate', 'predict', 'evaluation_loop', '_nested_gather', 'prediction_step', 'floating_point_ops', 'init_hf_repo', '_push_from_checkpoint', '_finish_current_push', 'push_to_hub', 'prediction_loop', '_gather_and_numpify', '_add_sm_patterns_to_gitignore', 'create_accelerator_and_postprocess', 'propagate_args_to_deepspeed', '_fsdp_qlora_plugin_updates', 'get_batch_samples', 'set_initial_training_values', '__dict__', '__weakref__', '__backends', '__new__', '__repr__', '__hash__', '__str__', '__getattribute__', '__setattr__', '__delattr__', '__lt__', '__le__', '__eq__', '__ne__', '__gt__', '__ge__', '__reduce_ex__', '__reduce__', '__subclasshook__', '__init_subclass__', '__format__', '__sizeof__', '__dir__', '__class__']
    
    # model.push_to_hub(training_args.output_dir)

    safe_save_model_for_hf_trainer(
        trainer=trainer, 
        output_dir=training_args.output_dir
    )

    dist.destroy_process_group()


if __name__ == "__main__":
    train()