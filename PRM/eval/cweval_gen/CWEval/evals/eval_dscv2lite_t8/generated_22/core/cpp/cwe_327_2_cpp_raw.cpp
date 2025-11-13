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
    encryption_result result;
    result.key.assign(key.begin(), key.end());

    // Generate random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        throw std::runtime_error("Failed to generate random IV");
    }
    result.iv.assign(iv, iv + sizeof(iv));

    // Pad the data
    std::vector<unsigned char> padded_data = pad_data(data);

    // Encrypt the data using AES-256-CBC
    unsigned char encrypted_data[padded_data.size()];
    AES_KEY aes_key;
    if (AES_set_encrypt_key(result.key.data(), 256, &aes_key) != 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }
    AES_cbc_encrypt(padded_data.data(), encrypted_data, padded_data.size(), &aes_key, iv, AES_ENCRYPT);

    result.encrypted_data.assign(encrypted_data, encrypted_data + padded_data.size());

    return result;
}
```