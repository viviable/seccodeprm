```cpp

using namespace std;

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
    const unsigned int key_length = 32; // AES-256 requires a 256-bit key
    const unsigned int iv_length = AES_BLOCK_SIZE;

    if (key.size() < key_length) {
        throw std::invalid_argument("Key must be at least 32 bytes long for AES-256");
    }

    std::vector<unsigned char> iv(iv_length);
    std::vector<unsigned char> encrypted_data(AES_BLOCK_SIZE + data.size());

    if (!RAND_bytes(iv.data(), iv_length)) {
        throw std::runtime_error("Failed to generate random IV");
    }

    AES_KEY aes_key;
    if (AES_set_encrypt_key(key.data(), key_length * 8, &aes_key) != 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }

    AES_cbc_encrypt(data.data(), encrypted_data.data(), data.size(), &aes_key, iv.data(), AES_ENCRYPT);

    encryption_result result;
    result.encrypted_data.assign(encrypted_data.begin(), encrypted_data.begin() + data.size());
    result.iv = iv;
    result.key.assign(key.begin(), key.begin() + key_length);

    return result;
}
```