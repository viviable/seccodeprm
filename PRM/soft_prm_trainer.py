import os
import textwrap
from itertools import chain
from pathlib import Path
from typing import Callable, Optional, Union

import torch
import torch.nn as nn
from accelerate import PartialState
from datasets import Dataset, features
from transformers import (
    BaseImageProcessor,
    DataCollator,
    FeatureExtractionMixin,
    PreTrainedModel,
    PreTrainedTokenizerBase,
    ProcessorMixin,
    Trainer,
    is_wandb_available,
)
from transformers.trainer_callback import TrainerCallback
from transformers.trainer_utils import EvalPrediction
from transformers.utils import is_peft_available

from trl.trainer.prm_config import PRMConfig
from trl.trainer.utils import compute_accuracy, disable_dropout_in_model, generate_model_card
from trl.models import prepare_peft_model


if is_peft_available():
    from peft import PeftModel

if is_wandb_available():
    import wandb


class SoftDataCollatorForTokenClassification(DataCollator):
    def __init__(
        self,
        tokenizer: PreTrainedTokenizerBase,
        padding: bool = True,
        max_length: Optional[int] = None,
        pad_to_multiple_of: Optional[int] = None,
        label_pad_token_id: float = -100.0,
        return_tensors: str = "pt",
    ):
        self.tokenizer = tokenizer
        self.padding = padding
        self.max_length = max_length
        self.pad_to_multiple_of = pad_to_multiple_of
        self.label_pad_token_id = label_pad_token_id
        self.return_tensors = return_tensors

    def __call__(self, features):
        import transformers.data.data_collator as dc

        label_name = "label" if "label" in features[0] else "labels"
        labels = [feature[label_name] for feature in features] if label_name in features[0] else None
        no_labels_features = [{k: v for k, v in feature.items() if k != label_name} for feature in features]

        batch = dc.pad_without_fast_tokenizer_warning(
            self.tokenizer,
            no_labels_features,
            padding=self.padding,
            max_length=self.max_length,
            pad_to_multiple_of=self.pad_to_multiple_of,
            return_tensors="pt",
        )

        if labels is None:
            return batch

        sequence_length = batch["input_ids"].shape[1]
        padding_side = self.tokenizer.padding_side

        def to_list(tensor_or_iterable):
            if isinstance(tensor_or_iterable, torch.Tensor):
                return tensor_or_iterable.tolist()
            return list(tensor_or_iterable)

        if padding_side == "right":
            padded = [
                to_list(label) + [self.label_pad_token_id] * (sequence_length - len(label)) for label in labels
            ]
        else:
            padded = [
                [self.label_pad_token_id] * (sequence_length - len(label)) + to_list(label) for label in labels
            ]

        batch[label_name] = torch.tensor(padded, dtype=torch.float32)
        if label_name != "labels":
            batch["labels"] = batch.pop(label_name)
        return batch


class SoftPRMTrainer(Trainer):
    _tag_names = ["trl", "prm", "soft"]

    def __init__(
        self,
        model: Optional[Union[PreTrainedModel, nn.Module]] = None,
        args: Optional[PRMConfig] = None,
        data_collator: Optional[DataCollator] = None,
        train_dataset: Optional[Dataset] = None,
        eval_dataset: Optional[Union[Dataset, dict[str, Dataset]]] = None,
        processing_class: Optional[
            Union[PreTrainedTokenizerBase, BaseImageProcessor, FeatureExtractionMixin, ProcessorMixin]
        ] = None,
        model_init: Optional[Callable[[], PreTrainedModel]] = None,
        compute_metrics: Optional[Callable[[EvalPrediction], dict]] = None,
        callbacks: Optional[list[TrainerCallback]] = None,
        optimizers: tuple[torch.optim.Optimizer, torch.optim.lr_scheduler.LambdaLR] = (
            None,
            None,
        ),
        preprocess_logits_for_metrics: Optional[Callable[[torch.Tensor, torch.Tensor], torch.Tensor]] = None,
        peft_config: Optional[dict] = None,
    ):
        if peft_config is not None or (is_peft_available() and isinstance(model, PeftModel)):
            model = prepare_peft_model(model, peft_config, args)

        if args.disable_dropout:
            disable_dropout_in_model(model)

        if compute_metrics is None:
            compute_metrics = compute_accuracy

        if data_collator is None:
            if processing_class is None:
                raise ValueError(
                    "A processing_class must be specified when using the default SoftDataCollatorForTokenClassification"
                )
            data_collator = SoftDataCollatorForTokenClassification(processing_class, max_length=args.max_length)

        self.label_pad_token_id = getattr(data_collator, "label_pad_token_id", -100.0)

        if "input_ids" not in train_dataset.column_names:
            with PartialState().main_process_first():
                fn_kwargs = {
                    "tokenizer": processing_class,
                    "step_separator": args.step_separator,
                    "max_length": args.max_length,
                    "max_prompt_length": args.max_prompt_length,
                    "max_completion_length": args.max_completion_length,
                    "train_on_last_step_only": args.train_on_last_step_only,
                }
                train_fn_kwargs = {**fn_kwargs, "is_eval": False}
                train_dataset = train_dataset.map(
                    self.tokenize_row,
                    fn_kwargs=train_fn_kwargs,
                    num_proc=args.dataset_num_proc,
                    remove_columns=train_dataset.features,
                    desc="Tokenizing train dataset",
                    features=features.Features(
                        {
                            "labels": features.Sequence(features.Value("float32")),
                            "input_ids": features.Sequence(features.Value("int64")),
                        }
                    ),
                )
                eval_fn_kwargs = {**fn_kwargs, "is_eval": True}
                if eval_dataset is not None:
                    eval_dataset = eval_dataset.map(
                        self.tokenize_row,
                        fn_kwargs=eval_fn_kwargs,
                        num_proc=args.dataset_num_proc,
                        remove_columns=eval_dataset.features,
                        desc="Tokenizing eval dataset",
                        features=features.Features(
                            {
                                "labels": features.Sequence(features.Value("float32")),
                                "input_ids": features.Sequence(features.Value("int64")),
                            }
                        ),
                    )

        super().__init__(
            model=model,
            args=args,
            data_collator=data_collator,
            train_dataset=train_dataset,
            eval_dataset=eval_dataset,
            processing_class=processing_class,
            model_init=model_init,
            compute_metrics=compute_metrics,
            callbacks=callbacks,
            optimizers=optimizers,
            preprocess_logits_for_metrics=preprocess_logits_for_metrics,
        )

        if hasattr(self.model, "add_model_tags"):
            self.model.add_model_tags(self._tag_names)

    @staticmethod
    def tokenize_row(
        features,
        tokenizer,
        step_separator,
        max_length,
        max_prompt_length,
        max_completion_length,
        train_on_last_step_only,
        is_eval,
    ):
        prompt_ids = tokenizer(features["prompt"], add_special_tokens=False)["input_ids"]
        completions_ids = [
            tokenizer(completion, add_special_tokens=False)["input_ids"] for completion in features["completions"]
        ]
        if train_on_last_step_only and not is_eval:
            labels = [-100.0] * (len(features["labels"]) - 1) + [float(features["labels"][-1])]
        else:
            labels = [float(label) for label in features["labels"]]

        separator_ids = tokenizer.encode(step_separator, add_special_tokens=False)
        completions_ids = [completion + separator_ids for completion in completions_ids]

        labels = [[-100.0] * (len(completion) - 1) + [label] for completion, label in zip(completions_ids, labels)]

        completion_ids = list(chain(*completions_ids))
        labels = list(chain(*labels))

        if tokenizer.bos_token_id is not None:
            prompt_ids = [tokenizer.bos_token_id] + prompt_ids

        if max_prompt_length is not None:
            prompt_ids = prompt_ids[-max_prompt_length:]
        if max_completion_length is not None:
            completion_ids = completion_ids[:max_completion_length]
            labels = labels[:max_completion_length]

        input_ids = prompt_ids + completion_ids
        labels = [float(-100.0)] * len(prompt_ids) + labels

        if max_length is not None:
            input_ids = input_ids[:max_length]
            labels = labels[:max_length]

        return {"input_ids": input_ids, "labels": labels}

    def compute_loss(self, model, inputs, return_outputs=False, **kwargs):
        labels = inputs.pop("labels")
        outputs = model(**inputs)
        logits = outputs.logits

        if logits.size(-1) != 2:
            raise ValueError("SoftPRMTrainer expects num_labels=2 for soft binary labels.")

        labels = labels.to(logits.device)
        mask = labels.ne(self.label_pad_token_id)
        probs = torch.clamp(labels, 0.0, 1.0)
        log_probs = torch.log_softmax(logits, dim=-1)
        target = torch.stack([1.0 - probs, probs], dim=-1)
        token_loss = -(target * log_probs).sum(dim=-1)
        token_loss = token_loss * mask
        denom = mask.sum().clamp(min=1)
        loss = token_loss.sum() / denom

        return (loss, outputs) if return_outputs else loss

    def _save_checkpoint(self, model, trial):
        if self.args.hub_model_id is None:
            model_name = Path(self.args.output_dir).name
        else:
            model_name = self.args.hub_model_id.split("/")[-1]
        self.create_model_card(model_name=model_name)
        super()._save_checkpoint(model, trial)

    def create_model_card(
        self,
        model_name: Optional[str] = None,
        dataset_name: Optional[str] = None,
        tags: Union[str, list[str], None] = None,
    ):
        if not self.is_world_process_zero():
            return

        if hasattr(self.model.config, "_name_or_path") and not os.path.isdir(self.model.config._name_or_path):
            base_model = self.model.config._name_or_path
        else:
            base_model = None

        if tags is None:
            tags = set()
        elif isinstance(tags, str):
            tags = {tags}
        else:
            tags = set(tags)

        if hasattr(self.model.config, "unsloth_version"):
            tags.add("unsloth")

        if "JOB_ID" in os.environ:
            tags.add("hf_jobs")

        tags.update(self._tag_names)

        citation = textwrap.dedent("""\
        @article{uesato2022solving,
            title        = {{Solving Math Word Problems With Process- and Outcome-Based Feedback}},
            author       = {Uesato, Jonathan and Kushman, Nate and Kumar, Ramana and Song, Francis and Siegel, Noah and Wang, Lisa and Creswell, Antonia and Irving, Geoffrey and Higgins, Irina},
            year         = 2022,
            journal      = {arXiv preprint arXiv:2211.14275}
        }""")

        model_card = generate_model_card(
            base_model=base_model,
            model_name=model_name,
            hub_model_id=self.hub_model_id,
            dataset_name=dataset_name,
            tags=tags,
            wandb_url=wandb.run.url if is_wandb_available() and wandb.run is not None else None,
            trainer_name="PRM",
            trainer_citation=citation,
            paper_title="Solving math word problems with process-and outcome-based feedback",
        )

        model_card.save(os.path.join(self.args.output_dir, "README.md"))
