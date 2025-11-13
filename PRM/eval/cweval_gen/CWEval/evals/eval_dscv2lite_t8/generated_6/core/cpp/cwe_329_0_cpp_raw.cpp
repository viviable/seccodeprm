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
    // Ensure the key is at least 32 bytes (AES-256)
    if (key.size() < 32) {
        throw std::invalid_argument("Key must be at least 32 bytes");
    }

    // Create an IV (Initialization Vector)
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the data to make it a multiple of the block size
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Create a buffer for the encrypted data
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Set up the AES encryption context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
    }
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize AES encryption");
    }

    int len;
    if (1 != EVP_EncryptUpdate(ctx, encrypted_data.data(), &len, padded_data.data(), padded_data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to update AES encryption");
    }
    int ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, encrypted_data.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize AES encryption");
    }
    ciphertext_len += len;

    // Resize the encrypted data buffer to the actual length
    encrypted_data.resize(ciphertext_len);

    // Clean up the EVP context
    EVP_CIPHER_CTX_free(ctx);

    // Return the encrypted data, IV, and key used for encryption
    return std::make_tuple(encrypted_data, iv, key);
}
```