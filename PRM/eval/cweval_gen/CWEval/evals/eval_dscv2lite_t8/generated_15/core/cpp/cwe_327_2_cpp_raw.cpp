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

    // Use the first 32 bytes of the key for AES-128
    std::vector<unsigned char> aes_key(key.begin(), key.begin() + 32);

    // Generate a random IV
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        throw runtime_error("Failed to generate IV");
    }

    // Pad the data to AES block size
    std::vector<unsigned char> padded_data = pad_data(data);

    // Encrypt the data
    result.iv.assign(iv, iv + AES_BLOCK_SIZE);
    result.key.assign(aes_key.begin(), aes_key.end());

    int len = 0;
    if (AES_set_encrypt_key(aes_key.data(), 128, &enc_key) != 0) {
        throw runtime_error("Failed to set AES encryption key");
    }
    AES_cbc_encrypt(padded_data.data(), result.encrypted_data.data(), padded_data.size(), &enc_key, iv, AES_ENCRYPT);

    return result;
}
```