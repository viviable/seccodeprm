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
    std::vector<unsigned char> padded_data = pad_data(data);
    unsigned char iv[AES_BLOCK_SIZE];
    AES_KEY enc_key;

    // Generate random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Set up encryption key
    if (AES_set_encrypt_key(key.data(), 256, &enc_key) != 0) {
        throw std::runtime_error("Failed to set encryption key");
    }

    // Encrypt data
    result.encrypted_data.resize(padded_data.size());
    AES_cbc_encrypt(padded_data.data(), result.encrypted_data.data(),
                    padded_data.size(), &enc_key, iv, AES_ENCRYPT);

    result.iv.assign(iv, iv + AES_BLOCK_SIZE);
    result.key.assign(key.begin(), key.begin() + 32); // Use first 256 bits of key

    return result;
}
```