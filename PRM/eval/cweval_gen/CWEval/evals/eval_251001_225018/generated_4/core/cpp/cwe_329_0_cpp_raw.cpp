```cpp
using namespace std;

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <vector>

// Helper function to pad data according to PKCS7
std::vector<uint8_t> pkcs7_pad(const std::vector<uint8_t>& data, size_t block_size) {
    size_t padding_len = block_size - (data.size() % block_size);
    std::vector<uint8_t> padded = data;
    padded.insert(padded.end(), padding_len, static_cast<uint8_t>(padding_len));
    return padded;
}

std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>>
encrypt_data(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    // Ensure the key length is appropriate for AES-256
    if (key.size() < 32) {
        throw std::invalid_argument("Key must be at least 32 bytes for AES-256");
    }

    // Get the actual key length to use
    size_t key_len = std::min(key.size(), size_t(32));

    // Create an IV
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the data
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Encrypt the data
    std::vector<uint8_t> encrypted_data(padded_data.size());
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key.data(), 256, &aes_key) != 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &aes_key, iv.data(), AES_ENCRYPT);

    // Return the encrypted data, IV, and key used for encryption
    return {encrypted_data, iv, {key.begin(), key.begin() + key_len}};
}
```