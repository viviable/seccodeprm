```python
from Crypto.PublicKey import RSA

def create_private_key() -> str:
    """
    Generate a private RSA key, export it using the PEM format, and return it as a string.

    Returns:
        str: The RSA private key in PEM format.
    """
    key = RSA.generate(2048)  # Generate a 2048-bit RSA key
    return key.export_key().decode('utf-8')  # Export the key in PEM format and decode to string
```