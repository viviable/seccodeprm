#!/usr/bin/env bash

# Run a suite of evaluation benches for a single model checkpoint and collect
# all logs under log/<sanitised-model-name>/.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
DEFAULT_LOG_ROOT="${SCRIPT_DIR}/log"

usage() {
  cat <<'EOF'
Usage: eval_all_bench.sh <model_path_or_id> [run_label]

Arguments:
  <model_path_or_id>  Filesystem path or HF identifier of the model checkpoint to benchmark.
  [run_label]         Optional label used to distinguish this run's log files (default: current timestamp).

Environment variables:
  LOG_ROOT            Override the base directory for logs (default: ${SCRIPT_DIR}/log).
  ENABLE_BENCHES      Comma-separated subset of benches to run. Supported values:
                      binary_allsteps,bigvul_dedup,sven_val,precisebugs,
                      primevul_paired,primevul_unpaired,reposvul.
  DISABLE_BENCHES     Comma-separated list of benches to skip.

Each launched bench runs via nohup in the background; monitor the generated
logs to check completion.
EOF
}

if [[ $# -lt 1 ]]; then
  usage
  exit 1
fi

MODEL="${1}"
RUN_LABEL="${2:-$(date +"%y%m%d_%H%M%S")}"

# Sanitise model name for use in filesystem path.
SANITISED_MODEL="$(echo "${MODEL%/}" | sed 's#[^A-Za-z0-9._-]#_#g')"

LOG_ROOT="${LOG_ROOT:-$DEFAULT_LOG_ROOT}"
LOG_DIR="${LOG_ROOT}/${SANITISED_MODEL}"
PORT=46003
NPROC=$(nvidia-smi | grep 80GB | wc -l)
mkdir -p "${LOG_DIR}"

export PYTORCH_CUDA_ALLOC_CONF="${PYTORCH_CUDA_ALLOC_CONF:-expandable_segments:True}"

declare -a TASK_MATRIX=(
  "bigvul_dedup|${NPROC}|${PORT}|code_bigvul_bench.py|-b 1 -d bigvul_dedup"
  "sven_val|${NPROC}|${PORT}|code_sven_bench.py|-d sven_val -t 0.1 -c softmax"
  "precisebugs|${NPROC}|${PORT}|code_precise_bench.py|-b 1"
  "primevul_paired|${NPROC}|${PORT}|code_primevul_bench.py|-b 1 -d primevul_test_paired -c allsteps"
  "primevul_unpaired|${NPROC}|${PORT}|code_primevul_bench.py|-b 1 -d primevul_test_unpaired"
  "reposvul|${NPROC}|${PORT}|code_repos_bench.py|-b 1 -d reposvul_test"
)

parse_csv() {
  local csv="$1"
  [[ -z "${csv}" ]] && return
  IFS=',' read -ra _parsed <<<"${csv}"
  printf '%s\n' "${_parsed[@]}"
}

mapfile -t ENABLE_FILTER < <(parse_csv "${ENABLE_BENCHES:-}")
mapfile -t DISABLE_FILTER < <(parse_csv "${DISABLE_BENCHES:-}")

is_enabled() {
  local bench="$1"
  local enabled=1

  if [[ ${#ENABLE_FILTER[@]} -gt 0 ]]; then
    enabled=0
    for item in "${ENABLE_FILTER[@]}"; do
      if [[ "${bench}" == "${item}" ]]; then
        enabled=1
        break
      fi
    done
  fi

  if [[ ${#DISABLE_FILTER[@]} -gt 0 ]]; then
    for item in "${DISABLE_FILTER[@]}"; do
      if [[ "${bench}" == "${item}" ]]; then
        enabled=0
        break
      fi
    done
  fi

  [[ "${enabled}" -eq 1 ]]
}

launch_bench() {
  local bench_name="$1"
  local nproc="$2"
  local master_port="$3"
  local script_name="$4"
  shift 4
  local bench_args=("$@")

  if ! is_enabled "${bench_name}"; then
    echo "[SKIP] ${bench_name}"
    return
  fi

  local log_file="${LOG_DIR}/${RUN_LABEL}_${bench_name}.log"
  local cmd_file="${LOG_DIR}/${RUN_LABEL}_${bench_name}.cmd"

  local torch_cmd=(torchrun "--nproc_per_node=${nproc}")
  if [[ -n "${master_port}" ]]; then
    torch_cmd+=("--master_port" "${master_port}")
  fi
  torch_cmd+=("${SCRIPT_DIR}/${script_name}" "-m" "${MODEL}")
  torch_cmd+=("${bench_args[@]}")

  printf '%q ' "${torch_cmd[@]}" > "${cmd_file}"
  printf '\n' >> "${cmd_file}"

  echo "[RUN] ${bench_name} -> ${log_file}"
  if "${torch_cmd[@]}" > "${log_file}" 2>&1; then
    echo "[DONE] ${bench_name}"
  else
    echo "[FAIL] ${bench_name}. Check log: ${log_file}" >&2
    return 1
  fi
}

for task in "${TASK_MATRIX[@]}"; do
  IFS='|' read -r name nproc port script args <<<"${task}"
  args_array=()
  if [[ -n "${args}" ]]; then
    set +e
    read -r -a args_array <<<"${args}"
    read_status=$?
    set -e
    if (( read_status != 0 )); then
      echo "[WARN] Failed to parse args for ${name}, raw string: ${args}" >&2
    fi
  fi
  launch_bench "${name}" "${nproc}" "${port}" "${script}" "${args_array[@]}"
done

echo
echo "All benches completed. Logs are under: ${LOG_DIR}"
