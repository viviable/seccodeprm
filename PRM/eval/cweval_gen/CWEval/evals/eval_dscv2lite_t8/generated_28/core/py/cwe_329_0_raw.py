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

    # Ensure the key length is compatible with AES (16, 24, or 32 bytes)
    key_length = len(key)
    if key_length not in [16, 24, 32]:
        raise ValueError("Key must be 16, 24, or 32 bytes long")

    # Extract the appropriate portion of the key
    key = key[:key_length]

    # Generate a random initialization vector
    iv = os.urandom(AES.block_size)

    # Create an AES cipher object with the key and IV
    cipher = AES.new(key, AES.MODE_CBC, iv)

    # Pad the data to make it a multiple of the block size
    padded_data = pad(data, AES.block_size)

    # Encrypt the padded data
    encrypted_data = cipher.encrypt(padded_data)

    return encrypted_data, iv, key
```