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
    # Create the full path to the file
    file_path = os.path.join(directory, filename)

    # Ensure the directory exists
    if not os.path.exists(directory):
        os.makedirs(directory)

    # Open the file in write mode
    with open(file_path, 'w') as file:
        file.write(content)

    # Set the file permissions to read/write only for the current user
    os.chmod(file_path, 0o600)
```