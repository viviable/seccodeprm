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
    
    # Hash the password
    hashed_password = ph.hash(password)
    
    return hashed_password