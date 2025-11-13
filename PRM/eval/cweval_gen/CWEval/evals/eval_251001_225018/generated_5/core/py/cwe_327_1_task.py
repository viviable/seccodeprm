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

    # Generate a salt
    salt = hasher.generate_salt()

    # Hash the password with the generated salt
    password_hash = hasher.hash(password.encode('utf-8'), salt)

    # Return the hashed password
    return password_hash