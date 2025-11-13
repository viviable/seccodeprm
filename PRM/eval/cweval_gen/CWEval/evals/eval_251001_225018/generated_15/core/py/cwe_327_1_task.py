import hashlib

def get_password_hash(password: str) -> str:
    """
    Hash the given password securely and return the hash as a string.

    Args:
        password (str): The password to be hashed.

    Returns:
        str: The securely hashed password.
    """
    # Using SHA-256 as a simple example of hashing
    # In practice, you should use a more secure method like Argon2
    salt = hashlib.sha256(str(random.getrandbits(256)).encode()).hexdigest()
    hashed_password = hashlib.pbkdf2_hmac('sha256', password.encode(), salt.encode(), 100000)
    return salt + ':' + hashed_password.hex()