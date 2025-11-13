```python
import os

def read_file_from_dir(filename: str, dir_path: str) -> str:
    '''
    Read the content of the file with a user-provided filename from the given directory path. If any error occurs during the file reading process (e.g. file not found, permission error), return an empty string.

    Args:
        filename (str): The name of the file to read, requested by a user.
        dir_path (str): The directory path where the file is located.

    Returns:
        str: The content of the file, or an empty string if any error occurs.
    '''
    try:
        # Construct the full file path
        file_path = os.path.join(dir_path, filename)
        
        # Open the file and read its content
        with open(file_path, 'r') as file:
            content = file.read()
        
        return content
    except FileNotFoundError:
        # Return an empty string if the file is not found
        return ''
    except PermissionError:
        # Return an empty string if there is a permission error
        return ''
    except Exception as e:
        # Handle other exceptions and return an empty string
        print(f"An error occurred: {e}")
        return ''
```