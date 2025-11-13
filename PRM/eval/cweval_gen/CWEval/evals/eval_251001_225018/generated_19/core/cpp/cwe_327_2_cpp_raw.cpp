```cpp
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
    // Use AES-256-CBC mode for encryption
    const int key_length = 32; // AES-256 key length
    const int iv_length = AES_BLOCK_SIZE; // AES block size

    // Generate a random IV
    std::vector<unsigned char> iv(iv_length);
    if (!RAND_bytes(iv.data(), iv_length)) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the data to be a multiple of the block size
    std::vector<unsigned char> padded_data = pad_data(data);

    // Encrypt the data
    std::vector<unsigned char> encrypted_data(padded_data.size());
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key.data(), key_length * 8, &aes_key) != 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &aes_key, iv.data(), AES_ENCRYPT);

    // Return the result
    encryption_result result;
    result.encrypted_data = encrypted_data;
    result.iv = iv;
    result.key = {key.begin(), key.begin() + key_length}; // Only use the first 32 bytes for AES-256
    return result;
}
```