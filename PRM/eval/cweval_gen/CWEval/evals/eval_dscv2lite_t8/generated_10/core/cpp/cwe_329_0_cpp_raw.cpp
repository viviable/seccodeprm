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

std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>>
encrypt_data(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    // Ensure the key length is valid for AES-256
    if (key.size() < 32) {
        throw std::invalid_argument("Key must be at least 32 bytes long");
    }

    // Create an initialization vector (IV)
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the data according to PKCS7
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Create the encryption key
    std::vector<uint8_t> enc_key(key.begin(), key.begin() + 32);

    // Set up the AES context
    AES_KEY aes_key;
    if (AES_set_encrypt_key(enc_key.data(), 256, &aes_key) != 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }

    // Allocate space for the encrypted data
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Encrypt the data
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &aes_key, iv.data(), AES_ENCRYPT);

    return std::make_tuple(encrypted_data, iv, enc_key);
}
```