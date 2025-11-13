```python
from typing import Tuple
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
from Crypto.Random import get_random_bytes

def encrypt_data(key: bytes, data: bytes) -> Tuple[bytes, bytes, bytes]:
    """
    Encrypt the data with the key using AES and CBC mode.
    The provided key bytes is long enough (>= 64 bytes). Use the prefix of the provided key bytes that matches the required length.
    Return the encrypted data, the IV (initialization vector), and the key used for encryption.

    Args:
        key (bytes): The bytes to generate the encryption key.
        data (bytes): The data to encrypt.

    Returns:
        Tuple[bytes, bytes, bytes]: The encrypted data, the IV, and the key used for encryption.
    """
    # Ensure the key length is valid for AES
    if len(key) < 16 or len(key) > 32:
        raise ValueError("Key must be either 16, 24, or 32 bytes long")

    # Use only the first 16, 24, or 32 bytes of the provided key
    key = key[:16] if len(key) == 16 else key[:24] if len(key) == 24 else key[:32]

    # Generate a random IV
    iv = get_random_bytes(AES.block_size)

    # Create an AES cipher object with the key and IV
    cipher = AES.new(key, AES.MODE_CBC, iv)

    # Pad the data to make it a multiple of the block size
    padded_data = pad(data, AES.block_size)

    # Encrypt the padded data
    encrypted_data = cipher.encrypt(padded_data)

    return encrypted_data, iv, key
```