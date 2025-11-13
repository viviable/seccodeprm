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
    encryption_result result;

    // Use AES-256-CBC mode for encryption
    const int key_length = 32; // AES-256 requires a 32-byte key
    const int iv_length = AES_BLOCK_SIZE;

    if (key.size() < key_length) {
        throw std::runtime_error("Key is too short for AES-256");
    }

    AES_KEY enc_key;
    unsigned char iv[iv_length];

    // Generate random IV
    if (!RAND_bytes(iv, iv_length)) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Initialize AES key
    if (AES_set_encrypt_key(key.data(), 8 * key_length, &enc_key) < 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }

    // Pad the data
    std::vector<unsigned char> padded_data = pad_data(data);
    size_t padded_len = padded_data.size();

    // Encrypt the data
    result.encrypted_data.resize(padded_len);
    AES_cbc_encrypt(padded_data.data(), result.encrypted_data.data(), padded_len, &enc_key, iv, AES_ENCRYPT);

    // Store the IV and key
    result.iv.assign(iv, iv + iv_length);
    result.key.assign(key.begin(), key.begin() + key_length);

    return result;
}
```