```python
import hashlib


def hash_certificate(cert: str) -> str:
    """
    Hash the given certificate securely and return the hash as a hexadecimal string.

    Args:
        cert (str): The certificate to be hashed.

    Returns:
        str: The securely hashed certificate as a hexadecimal string.
    """
    # Create a sha256 hash object
    sha_signature = hashlib.sha256(cert.encode()).hexdigest()
    return sha_signature
```