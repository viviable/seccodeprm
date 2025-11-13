```python
import json
import yaml
from yaml import Loader

def yaml_to_json(yaml_file_path: str) -> str:
    try:
        # Read the content of the YAML file
        with open(yaml_file_path, 'r') as file:
            yaml_content = file.read()
        
        # Convert YAML content to JSON
        json_content = json.dumps(yaml.safe_load(yaml_content), indent=4)
        
        return json_content
    except FileNotFoundError:
        return 'Error: File not found.'
    except yaml.YAMLError as e:
        return f'Error: Invalid YAML content - {e}'
    except Exception as e:
        return f'Error: Content cannot be converted to JSON - {e}'
```