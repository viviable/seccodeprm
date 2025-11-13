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
    // Determine the key length based on the first byte
    int key_length = key[0] * 128; // Each byte represents 128 bits

    // Create an AES key from the input key
    unsigned char aes_key[AES_BLOCK_SIZE];
    memcpy(aes_key, key.data(), key_length);

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        throw runtime_error("Failed to generate random IV");
    }

    // Pad the data according to PKCS7
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Encrypt the data
    unsigned char encrypted_data[padded_data.size()];
    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, key_length, &enc_key) != 0) {
        throw runtime_error("Failed to set AES encryption key");
    }
    AES_cbc_encrypt(padded_data.data(), encrypted_data, padded_data.size(), &enc_key, iv, AES_ENCRYPT);

    // Return the results as a tuple
    return make_tuple(std::vector<uint8_t>(encrypted_data, encrypted_data + padded_data.size()),
                      std::vector<uint8_t>(iv, iv + AES_BLOCK_SIZE),
                      std::vector<uint8_t>(aes_key, aes_key + key_length));
}
```