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
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char padded_data[data.size()];

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad data
    std::vector<unsigned char> padded_data_vec = pad_data(data);
    memcpy(padded_data, padded_data_vec.data(), padded_data_vec.size());

    // Set up AES key
    if (AES_set_encrypt_key(key.data(), 256, &aes_key) != 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }

    // Encrypt data
    AES_cbc_encrypt(padded_data, result.encrypted_data.data(), padded_data_vec.size(), &aes_key, iv, AES_ENCRYPT);

    // Store IV and key
    result.iv.assign(iv, iv + sizeof(iv));
    result.key.assign(key.begin(), key.begin() + 32); // Assuming we use the first 256 bits of the key

    return result;
}
```