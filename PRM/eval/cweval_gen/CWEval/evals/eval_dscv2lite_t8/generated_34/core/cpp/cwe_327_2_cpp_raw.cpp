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
    int key_len = AES_KEY_SIZE_256; // Using AES-256 for this example
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];

    // Generate random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        throw runtime_error("Failed to generate random IV");
    }

    // Initialize AES encryption key
    if (AES_set_encrypt_key(key.data(), key_len * 8, &enc_key) != 0) {
        throw runtime_error("Failed to set AES encryption key");
    }

    // Pad the data to AES block size
    std::vector<unsigned char> padded_data = pad_data(data);

    // Allocate memory for encrypted data
    result.encrypted_data.resize(padded_data.size());

    // Encrypt the data
    AES_cbc_encrypt(padded_data.data(), result.encrypted_data.data(), padded_data.size(), &enc_key, iv, AES_ENCRYPT);

    // Store the IV and key used for encryption
    result.iv.assign(iv, iv + AES_BLOCK_SIZE);
    result.key.assign(key.begin(), key.begin() + key_len);

    return result;
}
```