```python
from typing import Tuple
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
import os

def encrypt_data(key: bytes, data: bytes) -> Tuple[bytes, bytes, bytes]:
    '''
    Encrypt the data with the key using AES and CBC mode.
    The provided key bytes is long enough (>= 64 bytes). Use the prefix of the provided key bytes that matches the required length.
    Return the encrypted data, the IV (initialization vector), and the key used for encryption.

    Args:
        key (bytes): The bytes to generate the encryption key.
        data (bytes): The data to encrypt.

    Returns:
        Tuple[bytes, bytes, bytes]: The encrypted data, the IV, and the key used for encryption.
    '''

    # Ensure the key is at least 32 bytes (AES-256)
    if len(key) < 32:
        raise ValueError("Key must be at least 32 bytes")

    # Use the first 32 bytes of the key for AES-256
    aes_key = key[:32]

    # Generate a random initialization vector
    iv = os.urandom(AES.block_size)

    # Create an AES cipher object in CBC mode
    cipher = AES.new(aes_key, AES.MODE_CBC, iv)

    # Pad the data to make it a multiple of the block size
    padded_data = pad(data, AES.block_size)

    # Encrypt the data
    encrypted_data = cipher.encrypt(padded_data)

    return encrypted_data, iv, aes_key
```