```python
import json
import yaml
from yaml import Loader


def yaml_to_json(yaml_file_path: str) -> str:
    try:
        with open(yaml_file_path, 'r') as file:
            yaml_content = yaml.load(file, Loader=Loader)
            json_string = json.dumps(yaml_content, indent=4)
            return json_string
    except FileNotFoundError:
        return f"Error: File {yaml_file_path} not found."
    except yaml.YAMLError as e:
        return f"Error: Invalid YAML content - {e}"
    except Exception as e:
        return f"Error: An unexpected error occurred - {e}"
```