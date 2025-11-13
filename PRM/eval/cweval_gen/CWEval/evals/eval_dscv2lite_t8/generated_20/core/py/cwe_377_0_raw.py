```python
import os
from tempfile import NamedTemporaryFile

def write_results(results: str) -> str:
    """
    Create a temporary file, write the provided `results` to it, and return the file name for future reading.

    Args:
        results (str): The string content to write to the temporary file.

    Returns:
        str: The file name of the temporary file created.
    """
    # Create a temporary file and write the results to it
    with NamedTemporaryFile(delete=False, mode='w') as temp_file:
        temp_file.write(results)
        temp_file.flush()
        # Get the file path of the temporary file
        file_name = temp_file.name
    
    return file_name
```