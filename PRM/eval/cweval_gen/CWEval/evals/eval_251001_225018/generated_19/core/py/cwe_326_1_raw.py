```python
from Crypto.PublicKey import DSA

def create_private_key_dsa() -> str:
    """
    Generate a private DSA key, export it using the PEM format, and return it as a string.

    Returns:
        str: The DSA private key in PEM format.
    """
    # Generate a new DSA key pair
    key = DSA.generate(1024)
    
    # Export the private key in PEM format
    pem = key.export_key()
    
    # Return the private key as a string
    return pem.decode('utf-8')
```