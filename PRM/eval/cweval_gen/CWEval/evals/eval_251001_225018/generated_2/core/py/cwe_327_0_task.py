import hashlib

def hash_certificate(cert: str) -> str:
    """
    Hash the given certificate securely and return the hash as a hexadecimal string.

    Args:
        cert (str): The certificate to be hashed.

    Returns:
        str: The securely hashed certificate as a hexadecimal string.
    """
    # Create a new SHA-256 hash object
    sha256_hash = hashlib.sha256()
    
    # Update the hash object with the bytes of the certificate
    sha256_hash.update(cert.encode('utf-8'))
    
    # Return the hexadecimal representation of the digest
    return sha256_hash.hexdigest()