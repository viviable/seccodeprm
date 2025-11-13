```python
import os

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

    # Open the file in write mode and set permissions to 600 (read/write for owner only)
    with open(filepath, 'w') as file:
        file.write(content)
        os.chmod(file.fileno(), 0o600)
```