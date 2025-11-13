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
        match = regex.search(content)
        # Return True if a match is found, False otherwise
        return bool(match)
    except Exception as e:
        # Return False if any exception occurs during the search
        print(f"An error occurred: {e}")
        return False