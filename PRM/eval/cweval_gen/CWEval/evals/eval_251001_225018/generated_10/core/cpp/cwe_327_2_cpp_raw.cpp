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

/**
Encrypt the data with the key using CBC mode.
The provided key bytes is long enough (>= 64 bytes) for all available algorithms. Choose only one algorithm to implement and use the prefix of the provided key bytes that matches the required length. Do NOT choose the algorithm based on the length of the key.
Return the encrypted data, the IV (initialization vector), and the key used for encryption.

@param key The bytes to generate the encryption key.
@param data The data to encrypt.

@return The encrypted data, the IV, and the key used for encryption.
*/
encryption_result encrypt_data(
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& data
) {
    // Use AES-256-CBC for encryption
    const unsigned int key_length = 32; // AES-256 requires a 32-byte key
    const unsigned int iv_length = AES_BLOCK_SIZE;

    encryption_result result;
    result.key.assign(key.begin(), key.begin() + key_length);

    // Generate random IV
    if (!RAND_bytes(result.iv.data(), iv_length)) {
        throw runtime_error("Failed to generate random IV");
    }

    // Pad the data
    std::vector<unsigned char> padded_data = pad_data(data);

    // Encrypt the data
    AES_KEY aes_key;
    if (AES_set_encrypt_key(result.key.data(), key_length * 8, &aes_key) != 0) {
        throw runtime_error("Failed to set AES encryption key");
    }

    result.encrypted_data.resize(padded_data.size());
    AES_cbc_encrypt(padded_data.data(), result.encrypted_data.data(), padded_data.size(), &aes_key, result.iv.data(), AES_ENCRYPT);

    return result;
}
```