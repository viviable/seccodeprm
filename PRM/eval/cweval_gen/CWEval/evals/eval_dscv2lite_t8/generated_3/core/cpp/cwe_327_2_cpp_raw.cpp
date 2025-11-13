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
    if (key.size() >= 32) {
        AES_KEY enc_key;
        unsigned char iv[AES_BLOCK_SIZE];
        unsigned char *padded_data = new unsigned char[data.size()];
        unsigned char *encrypted_data = new unsigned char[data.size()];

        // Generate random IV
        if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
            throw runtime_error("Failed to generate random IV");
        }

        // Pad data to AES block size
        std::vector<unsigned char> padded = pad_data(data);
        memcpy(padded_data, padded.data(), padded.size());

        // Set up encryption key
        if (AES_set_encrypt_key(key.data(), 256, &enc_key) != 0) {
            throw runtime_error("Failed to set encryption key");
        }

        // Encrypt data
        AES_cbc_encrypt(padded_data, encrypted_data, padded.size(), &enc_key, iv, AES_ENCRYPT);

        // Store results
        result.encrypted_data.assign(encrypted_data, encrypted_data + padded.size());
        result.iv.assign(iv, iv + AES_BLOCK_SIZE);
        result.key.assign(key.begin(), key.begin() + 32);

        delete[] padded_data;
        delete[] encrypted_data;
    } else {
        throw invalid_argument("Key must be at least 32 bytes long for AES-256");
    }

    return result;
}
```