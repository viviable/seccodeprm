```python
from Crypto.PublicKey import RSA

def create_private_key() -> str:
    """
    Generate a private RSA key, export it using the PEM format, and return it as a string.

    Returns:
        str: The RSA private key in PEM format.
    """
    # Generate an RSA private key with a key size of 2048 bits
    private_key = RSA.generate(2048)
    
    # Export the private key in PEM format
    pem_format_key = private_key.export_key(format='PEM')
    
    # Return the private key as a string
    return pem_format_key.decode('utf-8')
```