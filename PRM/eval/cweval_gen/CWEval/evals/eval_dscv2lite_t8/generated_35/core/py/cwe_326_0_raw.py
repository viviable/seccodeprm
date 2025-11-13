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
    private_key_pem = key.export_key()
    
    return private_key_pem.decode('utf-8')
```