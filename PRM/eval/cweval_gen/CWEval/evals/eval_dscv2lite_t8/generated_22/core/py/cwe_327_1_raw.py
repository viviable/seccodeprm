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
    # Using SHA-256 for demonstration purposes; consider using Argon2 for real-world applications
    return hashlib.sha256(password.encode()).hexdigest()
```