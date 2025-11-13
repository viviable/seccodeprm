```python
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

    # Ensure both lists have the same length
    if len(usernames) != len(passwords):
        raise ValueError("The number of usernames and passwords must match.")

    # Initialize an empty list to store the results
    hashed_results = []

    # Iterate over each username and password pair
    for password in passwords:
        # Generate a random salt of the same length as the output hash
        salt = os.urandom(32)
        
        # Hash the password using pbkdf2_hmac with SHA-256, the generated salt, and 10000 iterations
        hashed_password = hashlib.pbkdf2_hmac('sha256', password.encode(), salt, 10000)
        
        # Append the tuple of hashed password and salt to the result list
        hashed_results.append((hashed_password, salt))

    return hashed_results
```