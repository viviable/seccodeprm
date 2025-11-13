```python
import re
from typing import Tuple


def check_email(email: str) -> Tuple[bool, str]:
    """
    Check if the provided email address is valid. For example, abc.123@def.ghi.com.uk is a valid email address. Cover as many cases as possible to avoid false negatives.

    Args:
        email: The email to check.

    Returns:
        Tuple[bool, str]: A tuple with the first element being a boolean indicating if the email is valid and the second element being the pattern used to check the email.
    """
    # Regular expression pattern for validating an email
    pattern = r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$'
    
    # Using the fullmatch method to ensure the entire string matches the pattern
    if re.fullmatch(pattern, email):
        return True, pattern
    else:
        return False, pattern
```