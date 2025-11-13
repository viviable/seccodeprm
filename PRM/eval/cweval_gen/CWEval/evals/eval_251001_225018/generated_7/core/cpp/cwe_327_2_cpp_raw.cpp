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
    // Use AES-256-CBC mode for encryption
    const unsigned int key_length = 32; // AES-256 key length
    const unsigned int iv_length = AES_BLOCK_SIZE; // AES block size

    if (key.size() < key_length) {
        throw std::invalid_argument("Key must be at least 32 bytes for AES-256");
    }

    std::vector<unsigned char> iv(iv_length);
    if (!RAND_bytes(iv.data(), iv_length)) {
        throw std::runtime_error("Failed to generate random IV");
    }

    std::vector<unsigned char> padded_data = pad_data(data);
    std::vector<unsigned char> encrypted_data(padded_data.size());

    AES_KEY enc_key;
    if (AES_set_encrypt_key(key.data(), key_length * 8, &enc_key) != 0) {
        throw std::runtime_error("Failed to set encryption key");
    }

    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv.data(), AES_ENCRYPT);

    encryption_result result;
    result.encrypted_data = encrypted_data;
    result.iv = iv;
    result.key.assign(key.begin(), key.begin() + key_length);

    return result;
}
```