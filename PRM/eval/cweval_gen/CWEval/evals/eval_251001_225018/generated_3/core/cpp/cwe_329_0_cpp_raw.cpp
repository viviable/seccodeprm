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
    // Ensure the key is at least 32 bytes long (AES-256)
    if (key.size() < 32) {
        throw std::invalid_argument("Key must be at least 32 bytes long");
    }

    // Create an initialization vector
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), AES_BLOCK_SIZE)) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the data to make it a multiple of the block size
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Prepare space for the encrypted data
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Set up the AES encryption context
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key.data(), 256, &enc_key) != 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }

    // Perform the encryption
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv.data(), AES_ENCRYPT);

    // Return the encrypted data, IV, and key
    return std::make_tuple(encrypted_data, iv, std::vector<uint8_t>(key.begin(), key.begin() + 32));
}
```