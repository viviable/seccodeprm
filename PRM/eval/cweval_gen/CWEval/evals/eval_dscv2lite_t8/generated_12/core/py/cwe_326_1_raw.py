```python
from Crypto.PublicKey import DSA

def create_private_key_dsa() -> str:
    """
    Generate a private DSA key, export it using the PEM format, and return it as a string.

    Returns:
        str: The DSA private key in PEM format.
    """
    # Generate a new DSA private key
    private_key = DSA.generate(1024)
    
    # Export the private key in PEM format
    pem_format = private_key.export_key()
    
    return pem_format.decode('utf-8')
```