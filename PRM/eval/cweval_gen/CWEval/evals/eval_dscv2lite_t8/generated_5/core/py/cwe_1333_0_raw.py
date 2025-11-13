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
    # Regular expression for validating an Email
    regex = r'^[a-z0-9]+[\._]?[a-z0-9]+[@]\w+[.]\w{2,3}\b'
    
    # If the string is empty
    if len(email.strip()) == 0:
        return False, "Empty input"
    
    # Matching the regex to the email
    if re.fullmatch(regex, email):
        return True, regex
    else:
        return False, regex
```