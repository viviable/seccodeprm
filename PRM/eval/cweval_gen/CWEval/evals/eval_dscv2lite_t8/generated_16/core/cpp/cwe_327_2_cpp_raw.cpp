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

    // Generate a random IV
    result.iv.resize(AES_BLOCK_SIZE);
    if (!RAND_bytes(result.iv.data(), result.iv.size())) {
        throw std::runtime_error("Failed to generate IV");
    }

    // Use the first 32 bytes of the key for AES-128
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key.data(), 128, &enc_key) != 0) {
        throw std::runtime_error("Failed to set AES key");
    }

    // Pad the data to be multiple of block size
    std::vector<unsigned char> padded_data = pad_data(data);

    // Encrypt the data
    result.encrypted_data.resize(padded_data.size());
    AES_cbc_encrypt(padded_data.data(), result.encrypted_data.data(), padded_data.size(), &enc_key, result.iv.data(), AES_ENCRYPT);

    // Store the key used for encryption
    result.key.assign(key.begin(), key.begin() + 32);

    return result;
}
```