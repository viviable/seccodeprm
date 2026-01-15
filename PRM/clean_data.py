import json
import re
from datasets import DatasetDict, load_dataset
from tqdm import tqdm
from collections import Counter


pattern_prev = {
    "return 0;",
    "return 0;\n}",
    "return 1;\n}",
    'return TRUE;\n}',
    'return FALSE;\n}',
    "return SC_SUCCESS;\n}",
    "return NJS_OK;\n}",
    "return PJ_SUCCESS;\n}",
    "return offset;\n}",
    "return status;\n}",
    "return;\n}",
    "return NULL;\n}",
    "return rc;\n}",
    "return false;\n}",
    "return OPJ_TRUE;\n}",
    "return r;\n}",
    "return res;\n}",
    "return GF_OK;\n}",
    "return retval;\n}",
    "return true;\n}",
    "return ret;\n}",
    "return err;\n}",
    "return result;\n}",
    "return result",
    'return -1;\n}',
    'return;',
    "return true;",
    "return false;",
    "return error;\n}",
    "break;",
    "LOG_FUNC_CALLED(card->ctx);",
    "virResetLastError();",
    "*/",
    "\"\"\"",
    "#else",
    "}",
    "#endif",
    "#endif /*GPAC_DISABLE_ISOM_WRITE*/",
    "",
}
pattern_next = {
    "MagickBooleanType\n    status;",
    "register ssize_t\n    i;",
    "register ssize_t\n      x;",
    "ssize_t\n    y;",
    "lock_sock(sk);",
    "#ifndef GPAC_DISABLE_ISOM_WRITE",
    "Image\n    *image;",
    "namespace {",
    "This program is distributed in the hope that it will be useful,\n   but WITHOUT ANY WARRANTY; without even the implied warranty of\n   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n   GNU General Public License for more details.",
    "This program is free software: you can redistribute it and/or modify\n   it under the terms of the GNU General Public License as published by\n   the Free Software Foundation, either version 3 of the License, or\n   (at your option) any later version.",
    "You should have received a copy of the GNU General Public License\n   along with this program.  If not, see <http://www.gnu.org/licenses/>.\n*/",
}
patterns = list(pattern_prev | pattern_next)
def preprocess_data(data):
    for item in data:
        for key, value in item.items():
            if value == [] or (isinstance(value, list) and len(value) == 0):
                print(f"Empty list found in column: {key}")
                item[key] = None
            if key == 'detail_info' and isinstance(value, dict):
                for k, v in value.items():
                    # Convert ALL empty lists to None
                    if v == [] or (isinstance(v, list) and len(v) == 0):
                        print(f"Empty list found in column: {k}")
                        item['detail_info'][k] = None
                    # Keep non-empty values as-is
                    # Remove the redundant else branches
                        
    return data

def load_data(language_name):
    json_path = f'/project/flame/wyu3/PRM/ReposVul/ReposVul_{language_name}.jsonl' 
    data = []
    
    with open(json_path, 'r') as f:
        for line in f:
            single_data = json.loads(line)
            single_data['detail_info'] = {}
            for i, detail in enumerate(single_data['details']):
                if 'target' in detail:
                    single_data['detail_info'][f'{i}'] = detail   
            if single_data['detail_info'] == {}: ## remove the details without target
                continue
            single_data.pop('details')  ## change detials to detail_info
            data.append(single_data)
   
    dataset = preprocess_data(data)

def main():
    ## 
    # Create DatasetDict once to hold all splits
    vul_dataset = DatasetDict()
    
    # Process each split
    language_name = ['c', 'cpp', 'java', 'python' ] #
    train_data = []
    test_data = [] 
    for split_name in language_name:
        print(f"Processing {split_name} split...")
        
        split_data = []
        prompt = 'Determine whether the code is vulnerable or not.'
        source = 'REPOSVUL'
        
        dataset = load_data(split_name)
        for index, data in enumerate(tqdm(dataset, desc=f"Processing {split_name}")):
            detail_info= json.loads(data['detail_info'])
            for i, i_detail in detail_info.items():
                outdated = i_detail.get('outdated')
                if outdated:
                    continue
                import pdb; pdb.set_trace()




def remove_empty_steps(example):
    completions = example.get("completions", [])
    labels = example.get("labels", [])
    if not completions or not labels:
        return example
    if len(completions) != len(labels):
        raise ValueError(
            f"Length mismatch: completions={len(completions)} labels={len(labels)} for index={example.get('index')}"
        )
    def is_drop_step(text):
        stripped = text.strip()
        if stripped == "":
            return True
        if stripped == "/**/":
            return True
        return re.fullmatch(r"(?:/\*\s*\*/|/\*\*\s*\*/|//\s*)", stripped) is not None

    filtered = [(c, l) for c, l in zip(completions, labels) if not is_drop_step(c)]
    if not filtered:
        return {**example, "completions": [], "labels": []}
    new_completions, new_labels = zip(*filtered)
    return {**example, "completions": list(new_completions), "labels": list(new_labels)}

def merge_steps_by_patterns(example):
    completions = example.get("completions", [])
    labels = example.get("labels", [])
    if not completions or not labels:
        return example
    if len(completions) != len(labels):
        raise ValueError(
            f"Length mismatch: completions={len(completions)} labels={len(labels)} for index={example.get('index')}"
        )

    

    merged_completions = []
    merged_labels = []
    i = 0
    while i < len(completions):
        step = completions[i]
        label = labels[i]

        stripped = step.strip()
        is_return = stripped.startswith("return") or stripped == "return;"
        is_else = stripped == "#else" or stripped.startswith("else")

        if (is_return or is_else or stripped in pattern_prev) and merged_labels and merged_labels[-1] == label:
            merged_completions[-1] = merged_completions[-1] + step
            i += 1
            continue

        if (
            stripped in pattern_next
            and i + 1 < len(completions)
            and labels[i + 1] == label
        ):
            completions[i + 1] = step + completions[i + 1]
            i += 1
            continue

        merged_completions.append(step)
        merged_labels.append(label)
        i += 1

    return {**example, "completions": merged_completions, "labels": merged_labels}

def count_empty_completions(ds, split):
    empty = 0
    total = 0
    for ex in ds[split]:
        completions = ex.get("completions", [])
        empty += sum(1 for c in completions if c == "")
        total += len(completions)
    return empty, total


def count_patterns(ds, patterns):
    pattern_counts = {p: 0 for p in patterns}
    for split in ds.keys():
        for ex in ds[split]:
            for completion in ex.get("completions", []):
                key = completion.strip()
                if key in pattern_counts:
                    pattern_counts[key] += 1
    return pattern_counts

def count_common_patterns(ds, top_n=20):
    counter = Counter()
    for split in ds.keys():
        for ex in ds[split]:
            counter.update([c.strip() for c in ex.get("completions", [])])
    return counter.most_common(top_n)

def clean_data():
    ds = load_dataset("vivi-yu/reposvul_processed_dataset")


    before_counts = count_patterns(ds, patterns)

    ds = ds.map(remove_empty_steps)
    ds = ds.map(merge_steps_by_patterns)
    ds.save_to_disk("/project/flame/wyu3/PRM/reposvul_processed_dataset_cleaned")

    after_counts = count_patterns(ds, patterns)

    print("Pattern counts (before -> after):")
    for p in patterns:
        print(repr(p), before_counts[p], "->", after_counts[p])
    
    
    print(count_common_patterns(ds))

clean_data()   
