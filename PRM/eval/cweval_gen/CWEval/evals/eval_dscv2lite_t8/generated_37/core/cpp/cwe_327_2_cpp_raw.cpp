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
    // Use AES-256-CBC for encryption
    const int key_length = 32; // AES-256 key length in bytes
    const int iv_length = AES_BLOCK_SIZE; // AES block size in bytes

    encryption_result result;
    result.key.assign(key.begin(), key.begin() + key_length);

    unsigned char iv[iv_length];
    if (!RAND_bytes(iv, iv_length)) {
        throw runtime_error("Failed to generate random IV");
    }
    result.iv.assign(iv, iv + iv_length);

    unsigned char* padded_data = new unsigned char[data.size()];
    memcpy(padded_data, data.data(), data.size());
    std::vector<unsigned char> padded_vector(padded_data, padded_data + data.size());
    delete[] padded_data;

    unsigned char encrypted_data[data.size()];
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key.data(), key_length * 8, &enc_key) != 0) {
        throw runtime_error("Failed to set AES encryption key");
    }

    AES_cbc_encrypt(padded_vector.data(), encrypted_data, padded_vector.size(), &enc_key, iv, AES_ENCRYPT);

    result.encrypted_data.assign(encrypted_data, encrypted_data + padded_vector.size());

    return result;
}
```