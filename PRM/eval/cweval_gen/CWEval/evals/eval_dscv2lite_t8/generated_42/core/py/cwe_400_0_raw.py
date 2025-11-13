```python
import re

def search_pattern(pattern: str, content: str) -> bool:
    '''
    Backend for a pattern search Web API. Searches for a user-provided pattern in a user-provided content. Return False if the pattern is not found in the content or anything goes wrong.

    Args:
        pattern (str): The pattern to search for, specified by a user.
        content (str): The content in which to search, specified by a user.

    Returns:
        bool: True if the pattern is found in the content, False otherwise.
    '''
    try:
        # Compile the regex pattern for better performance if this function is called multiple times with the same pattern
        regex = re.compile(pattern)
        # Search for the pattern in the content
        if regex.search(content):
            return True
        else:
            return False
    except re.error:
        # If there's an error in compiling the regex pattern, return False
        return False
```