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
    encryption_result result;
    unsigned char iv[AES_BLOCK_SIZE];
    AES_KEY enc_key;

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        throw runtime_error("Failed to generate random IV");
    }

    // Use the first 32 bytes of the key for AES-128
    if (AES_set_encrypt_key(key.data(), 128, &enc_key) < 0) {
        throw runtime_error("Failed to set AES encryption key");
    }

    // Pad the data
    std::vector<unsigned char> padded_data = pad_data(data);
    size_t padded_len = padded_data.size();

    // Allocate space for encrypted data
    result.encrypted_data.resize(padded_len);

    // Encrypt the data
    AES_cbc_encrypt(padded_data.data(), result.encrypted_data.data(), padded_len, &enc_key, iv, AES_ENCRYPT);

    // Store the IV and key
    result.iv.assign(iv, iv + AES_BLOCK_SIZE);
    result.key.assign(key.begin(), key.begin() + 32); // Only use the first 32 bytes of the key

    return result;
}
```