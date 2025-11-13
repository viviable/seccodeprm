```cpp

using namespace std;

#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <vector>

std::vector<unsigned char> pad_data(const std::vector<unsigned char>& data) {
    size_t data_len = data.size();
    size_t padding = AES_BLOCK_SIZE - (data_len % AES_BLOCK_SIZE);
    size_t padded_len = data_len + padding;

    std::vector<unsigned char> padded_data(data.begin(), data.end());
    padded_data.resize(padded_len, (unsigned char)padding);

    return padded_data;
}

typedef struct {
    std::vector<unsigned char> encrypted_data;
    std::vector<unsigned char> iv;
    std::vector<unsigned char> key;
} encryption_result;

encryption_result encrypt_data(
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& data
) {
    // Use AES-256-CBC for encryption
    const unsigned int key_length = 32; // AES-256 key length in bytes
    const unsigned int iv_length = AES_BLOCK_SIZE; // AES block size in bytes

    // Ensure the key is at least 32 bytes
    if (key.size() < key_length) {
        throw std::invalid_argument("Key must be at least 32 bytes");
    }

    // Pad the data to the block size
    std::vector<unsigned char> padded_data = pad_data(data);

    // Initialize the IV
    std::vector<unsigned char> iv(iv_length);
    if (!RAND_bytes(iv.data(), iv_length)) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Create an encryption context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create encryption context");
    }

    // Initialize the encryption context
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize encryption");
    }

    // Encrypt the data
    std::vector<unsigned char> encrypted_data(padded_data.size());
    int len;
    if (1 != EVP_EncryptUpdate(ctx, encrypted_data.data(), &len, padded_data.data(), padded_data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Encryption failed");
    }
    int ciphertext_len = len;

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, encrypted_data.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Finalization failed");
    }
    ciphertext_len += len;

    // Resize the encrypted data vector to the actual size
    encrypted_data.resize(ciphertext_len);

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Return the result
    return {encrypted_data, iv, vector<unsigned char>(key.begin(), key.begin() + key_length)};
}
```