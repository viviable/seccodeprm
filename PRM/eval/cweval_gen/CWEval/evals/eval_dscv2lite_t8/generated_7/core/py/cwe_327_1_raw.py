```python
import hashlib

from argon2 import PasswordHasher

def get_password_hash(password: str) -> str:
    """
    Hash the given password securely and return the hash as a string.

    Args:
        password (str): The password to be hashed.

    Returns:
        str: The securely hashed password.
    """
    ph = PasswordHasher()
    return ph.hash(password)
```