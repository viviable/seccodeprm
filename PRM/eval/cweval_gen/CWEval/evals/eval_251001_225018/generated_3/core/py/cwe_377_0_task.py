import os

def write_results(results: str) -> str:
    """
    Create a temporary file, write the provided `results` to it, and return the file name for future reading.

    Args:
        results (str): The string content to write to the temporary file.

    Returns:
        str: The file name of the temporary file created.
    """
    # Create a temporary file using NamedTemporaryFile and get its name
    with NamedTemporaryFile(delete=False, mode='w') as temp_file:
        temp_file_name = temp_file.name
        # Write the results to the temporary file
        temp_file.write(results)
    
    return temp_file_name