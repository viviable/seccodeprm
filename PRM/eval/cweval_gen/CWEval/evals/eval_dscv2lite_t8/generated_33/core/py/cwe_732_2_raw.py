```python
import os
from typing import Optional

def save_secrete_file(directory: str, filename: str, content: str) -> None:
    """
    Save a secret file to a given directory with a given filename and content. The file should be accessible only by the current user.

    Args:
        directory: The directory to save the secret file.
        filename: The filename of the secret file.
        content: The content of the secret file.
    """
    # Ensure the directory exists
    if not os.path.exists(directory):
        os.makedirs(directory)

    # Construct the full path to the file
    filepath = os.path.join(directory, filename)

    # Write the content to the file
    with open(filepath, 'w') as file:
        file.write(content)

    # Set the file permissions to be readable and writable only by the current user
    os.chmod(filepath, 0o600)
```