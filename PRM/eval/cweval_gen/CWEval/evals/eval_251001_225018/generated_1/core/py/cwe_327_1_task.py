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
    hasher = argon2.PasswordHasher()
    
    # Generate a salt and hash the password
    hashed_password = hasher.hash(password)
    
    return hashed_password