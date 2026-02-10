import json
import re
import ast
import os
import csv
import libcst as cst
from libcst.metadata import PositionProvider
from libcst._position import CodePosition
import shutil
import subprocess
from collections import OrderedDict

class CWE78Visitor(cst.CSTVisitor):
    METADATA_DEPENDENCIES = (PositionProvider,)

    def __init__(self, src, start, end):
        self.list_vars = set()
        self.src = src
        self.start = start
        self.end = end
        self.fp = False

    def visit_Assign(self, node):
        if len(node.targets) != 1: return
        if not isinstance(node.targets[0].target, cst.Name): return
        target_name = node.targets[0].target.value
        if isinstance(node.value, cst.List):
            if len(node.value.elements) == 0: return
            if not isinstance(node.value.elements[0].value, cst.BaseString): return
            self.list_vars.add(target_name)
        elif isinstance(node.value, cst.Name):
            if node.value.value in self.list_vars:
                self.list_vars.add(target_name)
        elif isinstance(node.value, cst.BinaryOperation):
            if isinstance(node.value.left, cst.List):
                self.list_vars.add(target_name)
            elif isinstance(node.value.left, cst.Name) and node.value.left.value in self.list_vars:
                self.list_vars.add(target_name)
            if isinstance(node.value.right, cst.List):
                self.list_vars.add(target_name)
            elif isinstance(node.value.right, cst.Name) and node.value.right.value in self.list_vars:
                self.list_vars.add(target_name)

    def visit_Name(self, node):
        pos = self.get_metadata(PositionProvider, node)
        if self.start.line != pos.start.line: return
        if self.start.column != pos.start.column: return
        if self.end.line != pos.end.line: return
        if self.end.column != pos.end.column: return
        assert pos.start.line == pos.end.line
        if node.value in self.list_vars:
            self.fp = True
            
def filter_cwe78_fps(s_out_dir):
    csv_path = os.path.join(s_out_dir, f'codeql.csv')
    out_src_dir = os.path.join(s_out_dir, f'output')
    with open(csv_path) as csv_f:
        lines = csv_f.readlines()
    shutil.copy2(csv_path, csv_path+'.fp')
    with open(csv_path, 'w') as csv_f:
        for line in lines:
            row = line.strip().split(',')
            if len(row) < 5: continue
            out_src_fname = row[-5].replace('/', '').strip('"')
            out_src_path = os.path.join(out_src_dir, out_src_fname)
            with open(out_src_path) as f:
                src = f.read()
            start = CodePosition(int(row[-4].strip('"')), int(row[-3].strip('"'))-1)
            end = CodePosition(int(row[-2].strip('"')), int(row[-1].strip('"')))
            visitor = CWE78Visitor(src, start, end)
            tree = cst.parse_module(src)
            wrapper = cst.MetadataWrapper(tree)
            wrapper.visit(visitor)
            if not visitor.fp:
                csv_f.write(line)

def codeql_create_db(info, out_src_dir, out_db_dir):
    if info['language'] == 'py':
        cmd = '/home/wyu3/workspace/sven/codeql/codeql database create {} --quiet --language=python --overwrite --source-root {}'
        cmd = cmd.format(out_db_dir, out_src_dir)
        subprocess.run(cmd, shell=True, stdout=subprocess.DEVNULL)
    elif info['language'] == 'c':
        cmd = '/home/wyu3/workspace/sven/codeql/codeql database create {} --quiet --language=cpp --overwrite --command="make -B" --source-root {}'
        cmd = cmd.format(out_db_dir, out_src_dir)
        subprocess.run(cmd, shell=True, stdout=subprocess.DEVNULL)
    else:
        raise NotImplementedError()

def codeql_analyze(info, out_db_dir, out_csv_path):
    # Convert relative path to absolute path if needed
    check_ql_path = info['check_ql']
    print(check_ql_path)
    if not os.path.isabs(check_ql_path):
        # Assuming the relative path is from the workspace/PURE/PRM/eval directory
        check_ql_path = os.path.abspath(os.path.join('/home/wyu3/workspace/sven/codeql', check_ql_path))
    
    if info['language'] == 'py':
        cmd = '/home/wyu3/workspace/sven/codeql/codeql database analyze {} {} --quiet --format=csv --output={} --additional-packs={}'
        cmd = cmd.format(out_db_dir, check_ql_path, out_csv_path, os.path.expanduser('~/.codeql/packages/codeql/'))
        subprocess.run(cmd, shell=True, stdout=subprocess.DEVNULL)
    elif info['language'] == 'c':
        cmd = '/home/wyu3/workspace/sven/codeql/codeql database analyze {} {} --quiet --format=csv --output={} --additional-packs={}'
        cmd = cmd.format(out_db_dir, check_ql_path, out_csv_path, os.path.expanduser('~/.codeql/packages/codeql/'))
        subprocess.run(cmd, shell=True, stdout=subprocess.DEVNULL)
    else:
        raise NotImplementedError()


class LMCodeEvaler:
    def __init__(self, sampling_params, llm):
        self.sampling_params = sampling_params
        self.llm = llm
        self.tokenizer = llm.get_tokenizer()
        
    def truncate(self, completion, lang):
        if lang == 'py':
            for match in re.finditer('\n', completion):
                cur_idx, next_idx = match.start(), match.end()
                if next_idx < len(completion) and not completion[next_idx].isspace():
                    completion = completion[:cur_idx]
                    break
            else:
                last_comment_str = '\n    #'
                if last_comment_str in completion:
                    completion = completion[:completion.rfind(last_comment_str)]
        elif lang == 'c':
            if '\n}' in completion:
                completion = completion[:completion.find('\n}')+2]
            else:
                last_comment_strs = ['\n    //', '\n    /*']
                for last_comment_str in last_comment_strs:
                    if last_comment_str in completion:
                        completion = completion[:completion.rfind(last_comment_str)]
                        completion = completion.rstrip() + '\n}'

            lines = completion.split('\n')
            final_lines = []
            for line in lines:
                if '->name = "' in line: continue
                final_lines.append(line)
            completion = '\n'.join(final_lines)
        else:
            raise NotImplementedError()

        return completion  
    
    def try_parse(self, code, lang):
        if lang == 'py':
            try:
                ast.parse(code)
                return 0
            except:
                return 1
        elif lang == 'c':
            cmd = 'gcc -c -x c -'
            process = subprocess.run(cmd, shell=True, timeout=5, input=code.encode(), stderr=subprocess.DEVNULL)
            if process.returncode == 0:
                return 0
            else:
                return 1
        else:
            raise NotImplementedError()
    
    def process_completions(self, input_src, input_ids_len, gen_output, generated_ids, lang):
        # tokens = gen_output[:, input_ids_len:, ...]
        # completions = self.tokenizer.batch_decode(tokens)
        completions = gen_output

        output_srcs, output_ids = [], []
        dup_srcs, non_parsed_srcs = [], []
        for i, completion in enumerate(completions):
            if self.tokenizer.eos_token in completion:
                completion = completion[:completion.find(self.tokenizer.eos_token)]
            completion = self.truncate(completion, lang)
            completion_len = len(self.tokenizer.encode(completion))
            output_src = input_src + completion
            output_src = output_src.rstrip() + '\n'
            if output_src in output_srcs:
                dup_srcs.append(output_src)
            elif self.try_parse(output_src, lang) != 0:
                non_parsed_srcs.append(output_src)
            else:
                output_srcs.append(output_src)
                output_ids.append((generated_ids[i][:input_ids_len], generated_ids[i][input_ids_len:input_ids_len+completion_len]))

        return output_srcs, output_ids, dup_srcs, non_parsed_srcs
               
    def generate_single(self, single_data):
        file_context = single_data['file_context']
        func_context = single_data['func_context']
        lang = single_data['info']['language']
        
        input_src = file_context + func_context
        input_ids = self.tokenizer(input_src, return_tensors='pt').input_ids
        input_ids_len = input_ids.shape[1]
        gen_output = self.llm.generate(
            input_src,
            sampling_params=self.sampling_params,
        )
        generated_text = [gen_output[0].outputs[i].text for i in range(self.sampling_params.n)]
        generated_ids = [gen_output[0].outputs[i].token_ids for i in range(self.sampling_params.n)]
        
        output_srcs, output_ids, dup_srcs, non_parsed_srcs = self.process_completions(input_src, input_ids_len, generated_text, generated_ids, lang)
        single_data['output_srcs'] = output_srcs
        single_data['dup_srcs'] = dup_srcs
        single_data['non_parsed_srcs'] = non_parsed_srcs
        
        return output_srcs, output_ids, dup_srcs, non_parsed_srcs
        
    def call_codeql(self, outputs, s_out_dir, single_data, out_src_dir, vul_type, all_fnames):
        vuls = set()
        if len(outputs) != 0:
            csv_path = os.path.join(s_out_dir, 'codeql.csv')
            db_path = os.path.join(s_out_dir, 'codeql_db')
            codeql_create_db(single_data['info'], out_src_dir, db_path)
            codeql_analyze(single_data['info'], db_path, csv_path)
            if vul_type == 'cwe-078':
                filter_cwe78_fps(s_out_dir)
            with open(csv_path) as csv_f:
                reader = csv.reader(csv_f)
                for row in reader:
                    if len(row) < 5: continue
                    out_src_fname = row[-5].replace('/', '')
                    vuls.add(out_src_fname)
        secs = all_fnames - vuls
        return secs, vuls
        
    def generate_testset_answers(self, testset, output_dir):
        os.makedirs(output_dir, exist_ok=True)
        results = {}
        for vul_type, data_scenario in testset.items():
            results[vul_type] = []
            s_out_dir = os.path.join(output_dir, vul_type)
            os.makedirs(s_out_dir, exist_ok=True)
            
            for scenario_single_data in data_scenario:
                for scenario, single_data in scenario_single_data.items():
                    s_out_dir = os.path.join(output_dir, vul_type, scenario)
                    os.makedirs(s_out_dir, exist_ok=True)
                    
                    outputs, output_ids, dup_srcs, non_parsed_srcs = self.generate_single(single_data)
                    
                    out_src_dir = os.path.join(s_out_dir, 'output')
                    os.makedirs(out_src_dir, exist_ok=True)
                    output_ids_j = OrderedDict()
                    all_fnames = set()
                    for i, (output, output_id) in enumerate(zip(outputs, output_ids)):
                        fname = f'{str(i).zfill(2)}.'+single_data['info']['language']
                        all_fnames.add(fname)
                        with open(os.path.join(out_src_dir, fname), 'w') as f:
                            f.write(output)
                        output_ids_j[fname] = output_id
                    with open(os.path.join(s_out_dir, f'output_ids.json'), 'w') as f:
                        json.dump(output_ids_j, f, indent=2)
                    if single_data['info']['language'] == 'c':
                        shutil.copy2('Makefile', out_src_dir)

                    for srcs, name in [(dup_srcs, 'dup'), (non_parsed_srcs, 'non_parsed')]:
                        src_dir = os.path.join(s_out_dir, f'{name}')
                        os.makedirs(src_dir, exist_ok=True)
                        for i, src in enumerate(srcs):
                            fname = f'{str(i).zfill(2)}.'+single_data['info']['language']
                            with open(os.path.join(src_dir, fname), 'w') as f:
                                f.write(src)

                    secs, vuls = self.call_codeql(outputs, s_out_dir, single_data, out_src_dir, vul_type, all_fnames)

                    d = OrderedDict()
                    d['vul_type'] = vul_type
                    d['scenario'] = scenario
                    d['total'] = len(all_fnames)
                    d['sec'] = len(secs)
                    d['vul'] = len(vuls)
                    d['dup'] = len(dup_srcs)
                    d['non_parsed'] = len(non_parsed_srcs)

                    
                    with open(os.path.join(s_out_dir, 'result.jsonl'), 'w') as f:
                        s = json.dumps(d)
                        f.write(s+'\n')
                        
                    # yield d
                    
                