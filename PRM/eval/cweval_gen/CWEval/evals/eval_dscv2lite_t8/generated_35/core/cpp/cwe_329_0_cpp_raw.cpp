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

std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>> encrypt_data(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    // Determine the appropriate key length for AES (128, 192, or 256 bits)
    size_t key_length = std::min(key.size(), size_t(32)); // AES-256 has a 32-byte key

    // Generate a random initialization vector (IV)
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the input data to make its length a multiple of the block size
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Create an array to hold the encrypted data
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Initialize AES encryption context
    AES_KEY aes_key;
    if (key_length == 16) {
        if (AES_set_encrypt_key(key.data(), 128, &aes_key) != 0) {
            throw std::runtime_error("Failed to set AES encryption key");
        }
    } else if (key_length == 24) {
        if (AES_set_encrypt_key(key.data(), 192, &aes_key) != 0) {
            throw std::runtime_error("Failed to set AES encryption key");
        }
    } else if (key_length == 32) {
        if (AES_set_encrypt_key(key.data(), 256, &aes_key) != 0) {
            throw std::runtime_error("Failed to set AES encryption key");
        }
    } else {
        throw std::invalid_argument("Invalid key length for AES");
    }

    // Encrypt the data using AES-CBC mode
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &aes_key, iv.data(), AES_ENCRYPT);

    // Return the encrypted data, IV, and key used for encryption
    return {encrypted_data, iv, {key.begin(), key.begin() + key_length}};
}
```