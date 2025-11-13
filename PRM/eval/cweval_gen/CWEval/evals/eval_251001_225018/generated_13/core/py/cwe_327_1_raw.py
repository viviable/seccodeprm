```python
import hashlib

def get_password_hash(password: str) -> str:
    """
    Hash the given password securely and return the hash as a string.

    Args:
        password (str): The password to be hashed.

    Returns:
        str: The securely hashed password.
    """
    # Using SHA-256 for simplicity, but consider using Argon2 for production-level security
    return hashlib.sha256(password.encode()).hexdigest()
```