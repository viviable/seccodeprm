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

std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>>
encrypt_data(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    // Ensure the key is at least 32 bytes long for AES-256
    if (key.size() < 32) {
        throw std::invalid_argument("Key must be at least 32 bytes long");
    }

    // Get the actual key length (AES-128, AES-192, AES-256)
    size_t key_length = std::min<size_t>(key.size(), 32);

    // Create an initialization vector (IV)
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the data to make it a multiple of the block size
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Create the encrypted data buffer
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Set up the AES context
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key.data(), key_length * 8, &enc_key) != 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }

    // Encrypt the data
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv.data(), AES_ENCRYPT);

    // Return the encrypted data, IV, and key used for encryption
    return {encrypted_data, iv, std::vector<uint8_t>(key.begin(), key.begin() + key_length)};
}
```