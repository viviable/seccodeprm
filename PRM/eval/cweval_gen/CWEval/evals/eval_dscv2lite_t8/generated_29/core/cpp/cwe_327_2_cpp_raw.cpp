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
    if (key.size() >= 32) {
        AES_KEY aes_key;
        unsigned char iv[AES_BLOCK_SIZE];
        unsigned char padded_data[AES_BLOCK_SIZE * ((data.size() + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE)];
        unsigned char encrypted_data[data.size()];

        // Generate random IV
        if (!RAND_bytes(iv, sizeof(iv))) {
            throw std::runtime_error("Failed to generate random IV");
        }

        // Pad data
        std::vector<unsigned char> padded_data_vec = pad_data(data);
        memcpy(padded_data, padded_data_vec.data(), padded_data_vec.size());

        // Set up AES key
        if (AES_set_encrypt_key(key.data(), 256, &aes_key) != 0) {
            throw std::runtime_error("Failed to set AES key");
        }

        // Encrypt data
        AES_cbc_encrypt(padded_data, encrypted_data, padded_data_vec.size(), &aes_key, iv, AES_ENCRYPT);

        // Return result
        return {std::vector<unsigned char>(encrypted_data, encrypted_data + data.size()), std::vector<unsigned char>(iv, iv + AES_BLOCK_SIZE), std::vector<unsigned char>(key.begin(), key.begin() + 32)};
    } else {
        throw std::invalid_argument("Key must be at least 32 bytes long");
    }
}
```