```python
import hashlib
import argon2

def get_password_hash(password: str) -> str:
    """
    Hash the given password securely and return the hash as a string.

    Args:
        password (str): The password to be hashed.

    Returns:
        str: The securely hashed password.
    """
    # Create an Argon2 context with default parameters
    ph = argon2.PasswordHasher()
    
    # Generate a salt
    salt = ph.generate_salt()
    
    # Hash the password using Argon2
    hashed_password = ph.hash(password.encode('utf-8'), salt)
    
    return hashed_password
```