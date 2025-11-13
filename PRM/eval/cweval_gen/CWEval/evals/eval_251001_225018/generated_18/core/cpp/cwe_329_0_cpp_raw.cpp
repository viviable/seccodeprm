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
    // Determine the key length (128, 192, or 256 bits)
    size_t key_length = std::min(key.size(), size_t(32)); // 32 bytes max for AES-256
    if (key_length == 16) {
        key_length *= 8; // Convert bytes to bits
    } else if (key_length == 24) {
        key_length *= 8; // Convert bytes to bits
    } else if (key_length == 32) {
        key_length *= 8; // Convert bytes to bits
    } else {
        throw std::invalid_argument("Key length must be 16, 24, or 32 bytes.");
    }

    // Generate a random IV
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV.");
    }

    // Pad the data
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Create an AES key structure
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key.data(), key_length, &enc_key) != 0) {
        throw std::runtime_error("Failed to set AES encryption key.");
    }

    // Allocate memory for the encrypted data
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Encrypt the data
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv.data(), AES_ENCRYPT);

    return {encrypted_data, iv, std::vector<uint8_t>(key.begin(), key.begin() + key_length / 8)};
}
```