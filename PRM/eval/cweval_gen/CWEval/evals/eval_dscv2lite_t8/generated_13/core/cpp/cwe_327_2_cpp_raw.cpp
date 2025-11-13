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
    result.key.assign(key.begin(), key.begin() + AES_KEY_LENGTH); // Using AES

    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        throw std::runtime_error("Failed to generate IV");
    }
    result.iv.assign(iv, iv + sizeof(iv));

    std::vector<unsigned char> padded_data = pad_data(data);
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key.data(), 8 * AES_KEY_LENGTH, &enc_key) != 0) {
        throw std::runtime_error("Failed to set encryption key");
    }

    result.encrypted_data.resize(padded_data.size());
    AES_cbc_encrypt(padded_data.data(), result.encrypted_data.data(), padded_data.size(), &enc_key, iv, AES_ENCRYPT);

    return result;
}
```