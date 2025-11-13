```python
from Crypto.PublicKey import DSA

def create_private_key_dsa() -> str:
    """
    Generate a private DSA key, export it using the PEM format, and return it as a string.

    Returns:
        str: The DSA private key in PEM format.
    """
    # Generate a new DSA private key
    dsa = DSA.generate(1024)
    
    # Export the private key in PEM format
    pem_format_key = dsa.export_key()
    
    return pem_format_key.decode('utf-8')
```