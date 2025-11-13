import hashlib
from typing import List, Tuple


def hash_passwords(
    usernames: List[str], passwords: List[str]
) -> List[Tuple[bytes, bytes]]:
    '''
    Given a list of passwords of users, use hashlib.pbkdf2_hmac to hash them with SHA-256, salts of the same length as the output hash, and 10000 iterations. Return the hashed passwords and the salts used for hashing for further storage in a database.

    Args:
        usernames: A list of usernames.
        passwords: A list of passwords of the users to hash.

    Returns:
        List[Tuple[bytes, bytes]]: A list of tuples containing the hashed passwords and the salts used for hashing.
    '''

    def generate_salt() -> bytes:
        """Generate a salt using SHA-256"""
        return hashlib.sha256(os.urandom(32)).digest()

    def hash_password(password: str, salt: bytes) -> bytes:
        """Hash a password using SHA-256, the provided salt, and 10000 iterations"""
        return hashlib.pbkdf2_hmac('sha256', password.encode(), salt, 10000)

    result = []
    for password in passwords:
        salt = generate_salt()
        hashed_password = hash_password(password, salt)
        result.append((hashed_password, salt))

    return result