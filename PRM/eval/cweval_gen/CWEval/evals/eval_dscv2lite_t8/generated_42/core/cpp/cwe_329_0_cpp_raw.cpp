```cpp

using namespace std;

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
    // Determine the effective key length based on the block size of AES
    size_t key_length = AES_BLOCK_SIZE; // AES-128 uses a 128-bit key
    if (key.size() >= AES_BLOCK_SIZE * 2) {
        key_length = AES_BLOCK_SIZE * 2; // AES-256 uses a 256-bit key
    } else if (key.size() >= AES_BLOCK_SIZE) {
        key_length = AES_BLOCK_SIZE; // AES-128 uses a 128-bit key
    }

    // Extract the appropriate prefix of the key
    std::vector<uint8_t> effective_key(key.begin(), key.begin() + key_length);

    // Generate a random IV
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the data to make it a multiple of the block size
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Encrypt the data
    std::vector<uint8_t> encrypted_data(padded_data.size());
    AES_KEY aes_key;
    if (AES_set_encrypt_key(effective_key.data(), key_length * 8, &aes_key) != 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &aes_key, iv.data(), AES_ENCRYPT);

    return std::make_tuple(encrypted_data, iv, effective_key);
}
```