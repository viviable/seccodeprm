```cpp

using namespace std;

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <vector>
#include <tuple>

// Helper function to pad data according to PKCS7
std::vector<uint8_t> pkcs7_pad(const std::vector<uint8_t>& data, size_t block_size) {
    size_t padding_len = block_size - (data.size() % block_size);
    std::vector<uint8_t> padded = data;
    padded.insert(padded.end(), padding_len, static_cast<uint8_t>(padding_len));
    return padded;
}

/**
Encrypt the data with the key using AES and CBC mode.
The provided key bytes is long enough (>= 64 bytes). Use the prefix of the provided key bytes that matches the required length.
Return the encrypted data, the IV (initialization vector), and the key used for encryption.

@param key The bytes to generate the encryption key
@param data The data to encrypt
@return tuple containing: {encrypted data, IV, key used for encryption}
*/
std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>>
encrypt_data(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    // Determine the key size based on the provided key bytes
    size_t key_size = AES_BLOCK_SIZE; // AES block size is typically 16 bytes
    if (key.size() >= 32 * AES_BLOCK_SIZE) {
        key_size = 32 * AES_BLOCK_SIZE; // 256-bit key
    } else if (key.size() >= 24 * AES_BLOCK_SIZE) {
        key_size = 24 * AES_BLOCK_SIZE; // 192-bit key
    } else if (key.size() >= 16 * AES_BLOCK_SIZE) {
        key_size = 16 * AES_BLOCK_SIZE; // 128-bit key
    }

    // Create an AES key schedule
    AES_KEY enc_key;
    int ret = AES_set_encrypt_key(key.data(), key_size * 8, &enc_key);
    if (ret != 0) {
        throw runtime_error("Failed to set AES encryption key");
    }

    // Generate a random initialization vector (IV)
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), AES_BLOCK_SIZE)) {
        throw runtime_error("Failed to generate random IV");
    }

    // Pad the data according to PKCS7
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Encrypt the data
    std::vector<uint8_t> encrypted_data(padded_data.size());
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv.data(), AES_ENCRYPT);

    return std::make_tuple(encrypted_data, iv, std::vector<uint8_t>(key.begin(), key.begin() + key_size));
}
```