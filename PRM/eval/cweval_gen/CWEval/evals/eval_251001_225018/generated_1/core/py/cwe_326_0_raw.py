```python
from Crypto.PublicKey import RSA

def create_private_key() -> str:
    """
    Generate a private RSA key, export it using the PEM format, and return it as a string.

    Returns:
        str: The RSA private key in PEM format.
    """
    # Generate a new RSA private key with a key size of 2048 bits
    key = RSA.generate(2048)
    
    # Export the private key in PEM format
    pem_key = key.export_key()
    
    # Return the private key as a string
    return pem_key.decode('utf-8')
```