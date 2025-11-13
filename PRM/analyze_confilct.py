import json
from datasets import load_from_disk

def find_conflicting_labels(dataset1, dataset2):
    """
    Find completion steps that appear in both datasets but with different labels.
    
    Args:
        dataset1: List of dicts with keys: 'prompt', 'completions', 'labels', 'index'
        dataset2: List of dicts with keys: 'prompt', 'completions', 'labels', 'index'
    
    Returns:
        List of conflicts with details about the mismatched steps
    """
    conflicts = []
    
    # Build a mapping from completion steps to their labels and source info for dataset2
    dataset2_steps = {}
    for data2_idx, data2 in enumerate(dataset2):
        for step_idx, (step, label) in enumerate(zip(data2['completions'], data2['labels'])):
            if step == '':
                continue
            if step not in dataset2_steps:
                dataset2_steps[step] = []
            dataset2_steps[step].append({
                'data_index': data2.get('index', data2_idx),
                'step_index': step_idx,
                'label': label,
                'code': data2['completions'][step_idx],
            })
    
    # Check dataset1 steps against dataset2
    for data1_idx, data1 in enumerate(dataset1):
        for step_idx, (step, label1) in enumerate(zip(data1['completions'], data1['labels'])):
            if step == '':
                continue
            if step in dataset2_steps:
                for match in dataset2_steps[step]:
                    label2 = match['label']
                    # Check if labels differ
                    if label1 != label2:
                        conflicts.append({
                            'step': step,
                            'dataset1_info': {
                                'data_index': data1.get('index', data1_idx),
                                'step_index': step_idx,
                                'label': label1,
                                'code': data1['completions'][step_idx],
                            },
                            'dataset2_info': match
                        })
    
    return conflicts


def save_conflicts(conflicts):
    with open("conflicts.jsonl", "w") as f:
        for conflict in conflicts:
            f.write(json.dumps(conflict) + "\n")


# Example usage:
if __name__ == "__main__":
    # Example dataset 1
    dataset1 = load_from_disk("/project/flame/wyu3/PRM/primevul_processed_dataset")["train"]
    dataset2 = load_from_disk("/project/flame/wyu3/PRM/sven_processed_dataset")["train"]
    
    # Find conflicts
    conflicts = find_conflicting_labels(dataset1, dataset2)
    print(f"\nSummary: {len(conflicts)} total conflicts found")
    
    # Print results
    save_conflicts(conflicts[:1000])
    
    # You can also get a summary