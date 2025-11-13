```python
from typing import Tuple

from Crypto.Cipher import AES
from Crypto.Util.Padding import pad


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
    # Ensure the key length is appropriate for AES
    key_length = AES.block_size * 8  # AES key lengths can be 128, 192, or 256 bits
    if len(key) < key_length:
        raise ValueError("Key must be at least {} bytes long".format(key_length))
    
    # Generate a random IV
    iv = os.urandom(AES.block_size)
    
    # Create an AES cipher object with the specified key and IV
    cipher = AES.new(key[:key_length], AES.MODE_CBC, iv)
    
    # Pad the data to make it a multiple of the block size
    padded_data = pad(data, AES.block_size)
    
    # Encrypt the data
    encrypted_data = cipher.encrypt(padded_data)
    
    return encrypted_data, iv, key[:key_length]
```