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
    if (key.size() < AES_KEY_LENGTH * 8) {
        throw std::invalid_argument("Key must be at least 128 bits");
    }

    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char* padded_data = new unsigned char[data.size()];
    memcpy(padded_data, data.data(), data.size());

    // Pad data to AES block size
    std::vector<unsigned char> padded_vec = pad_data(padded_data, data.size());
    delete[] padded_data;

    // Generate random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        throw std::runtime_error("Failed to generate IV");
    }

    // Set up AES key
    if (AES_set_encrypt_key(key.data(), AES_KEY_LENGTH * 8, &enc_key) != 0) {
        throw std::runtime_error("Failed to set AES key");
    }

    // Encrypt data
    result.encrypted_data.resize(padded_vec.size());
    AES_cbc_encrypt(padded_vec.data(), result.encrypted_data.data(), padded_vec.size(), &enc_key, iv, AES_ENCRYPT);

    // Store IV and key
    result.iv.assign(iv, iv + AES_BLOCK_SIZE);
    result.key.assign(key.begin(), key.begin() + AES_KEY_LENGTH * 8);

    return result;
}
```