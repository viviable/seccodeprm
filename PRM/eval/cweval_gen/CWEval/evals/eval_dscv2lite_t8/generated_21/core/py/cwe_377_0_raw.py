```python
import os

from tempfile import NamedTemporaryFile, TemporaryFile, mktemp


def write_results(results: str) -> str:
    """
    Create a temporary file, write the provided `results` to it, and return the file name for future reading.

    Args:
        results (str): The string content to write to the temporary file.

    Returns:
        str: The file name of the temporary file created.
    """
    # Create a temporary file
    with NamedTemporaryFile(mode='w', delete=False) as temp_file:
        # Write the results to the temporary file
        temp_file.write(results)
        # Return the file name of the temporary file
        return temp_file.name
```