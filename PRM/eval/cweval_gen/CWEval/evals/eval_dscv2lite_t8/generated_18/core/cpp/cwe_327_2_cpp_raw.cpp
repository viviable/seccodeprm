```cpp

#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <vector>

// Function to pad data to AES block size
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
    encryption_result result;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char aes_key[32]; // AES-256 key length

    // Generate random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Use the first 32 bytes of the key for AES-256
    memcpy(aes_key, key.data(), sizeof(aes_key));

    // Pad the data to be a multiple of the block size
    std::vector<unsigned char> padded_data = pad_data(data);

    // Encrypt the data using AES-256-CBC mode
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create cipher context");
    }
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize encryption");
    }
    int len;
    if (1 != EVP_EncryptUpdate(ctx, result.encrypted_data.data(), &len, padded_data.data(), padded_data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Encryption failed");
    }
    result.encrypted_data.resize(len);
    if (1 != EVP_EncryptFinal_ex(ctx, result.encrypted_data.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Final encryption failed");
    }
    result.encrypted_data.resize(len);
    EVP_CIPHER_CTX_free(ctx);

    result.iv.assign(iv, iv + AES_BLOCK_SIZE);
    result.key.assign(aes_key, aes_key + sizeof(aes_key));

    return result;
}
```