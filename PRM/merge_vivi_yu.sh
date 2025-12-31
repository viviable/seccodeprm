#!/usr/bin/env bash
set -euo pipefail

# Merge Vivi-Yu PRM checkpoints from Hugging Face.
# MODE=all merges every parameter; MODE=head merges only the classification head.
# Override MODELS, OUTPUT_DIR, MODE, and HEAD_KEYS (space-separated) as needed.

cd "$(dirname "$0")"

MODE=${MODE:-all}
OUTPUT_DIR=${OUTPUT_DIR:-"./outputs/merged_vivi_yu_${MODE}"}

# Replace or extend with the Vivi-Yu PRM checkpoints you want to average.
MODELS=(
  "vivi-yu/sven_s2_f1_71"
  # "vivi-yu/another_prm_checkpoint"
)

CMD=(python model_merge.py --models "${MODELS[@]}" --mode "$MODE" --output "$OUTPUT_DIR")

if [[ -n "${HEAD_KEYS:-}" ]]; then
  read -r -a HEAD_KEYS_ARR <<<"${HEAD_KEYS}"
  CMD+=("--head-keys" "${HEAD_KEYS_ARR[@]}")
fi

echo "Merging checkpoints: ${MODELS[*]}"
echo "Mode: ${MODE}, output: ${OUTPUT_DIR}"
exec "${CMD[@]}" "$@"
