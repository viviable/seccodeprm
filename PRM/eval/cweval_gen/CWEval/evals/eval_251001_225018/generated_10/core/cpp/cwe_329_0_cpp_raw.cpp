```cpp
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <vector>
#include <tuple>

// Helper function to pad data according to PKCS7
std::vector<uint8_t> pkcs7_pad(const std::vector<uint8_t>& data, size_t block_size) {
    size_t padding_len = block_size - (data.size() % block_size);
    std::vector<uint8_t> padded = data;
    padded.insert(padded.end(), padding_len, static_cast<uint8_t>(padding_len));
    return padded;
}

/**
Encrypt the data with the key using AES and CBC mode.
The provided key bytes is long enough (>= 64 bytes). Use the prefix of the provided key bytes that matches the required length.
Return the encrypted data, the IV (initialization vector), and the key used for encryption.

@param key The bytes to generate the encryption key
@param data The data to encrypt
@return tuple containing: {encrypted data, IV, key used for encryption}
*/
std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>>
encrypt_data(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    // Determine the key length (128, 192, or 256 bits)
    size_t key_length = key.size() * 8;
    if (key_length != 128 && key_length != 192 && key_length != 256) {
        throw std::invalid_argument("Key must be 16, 24, or 32 bytes long");
    }

    // Pad the data to make it a multiple of the block size (16 bytes for AES)
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Generate a random IV
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Create an AES key from the provided key bytes
    AES_KEY aes_key;
    int ret = AES_set_encrypt_key(key.data(), key_length, &aes_key);
    if (ret != 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }

    // Allocate space for the encrypted data
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Encrypt the data using AES-CBC mode
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &aes_key, iv.data(), AES_ENCRYPT);

    // Return the encrypted data, IV, and key used for encryption
    return std::make_tuple(encrypted_data, iv, key);
}
```