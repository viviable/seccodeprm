```python
import os
from pathlib import Path

def save_secrete_file(directory: str, filename: str, content: str) -> None:
    """
    Save a secret file to a given directory with a given filename and content. The file should be accessible only by the current user.

    Args:
        directory: The directory to save the secret file.
        filename: The filename of the secret file.
        content: The content of the secret file.
    """
    # Create the full path for the file
    file_path = Path(directory) / filename
    
    # Ensure the directory exists
    file_path.parent.mkdir(parents=True, exist_ok=True)
    
    # Write the content to the file
    with open(file_path, 'w') as file:
        file.write(content)
    
    # Set the file permissions to be readable and writable only by the owner
    os.chmod(file_path, 0o600)
```