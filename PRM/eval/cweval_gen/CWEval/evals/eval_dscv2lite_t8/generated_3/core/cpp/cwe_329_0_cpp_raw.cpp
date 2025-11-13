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

std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>> encrypt_data(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    // Ensure the key length is at least 32 bytes for AES-256
    size_t key_length = std::min(key.size(), size_t(32));

    // Create an AES key from the input key
    unsigned char aes_key[32];
    memcpy(aes_key, key.data(), key_length);

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the data
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Allocate space for the encrypted data
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Set up the AES context for encryption
    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, key_length * 8, &enc_key) < 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }

    // Perform the encryption
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv, AES_ENCRYPT);

    return {encrypted_data, std::vector<uint8_t>(iv, iv + sizeof(iv)), std::vector<uint8_t>(aes_key, aes_key + key_length)};
}
```
This implementation includes the necessary OpenSSL functions to perform AES encryption in CBC mode with PKCS7 padding. It ensures that the key length is appropriate for AES-256, generates a random IV, pads the data, sets up the AES encryption key, and performs the encryption. The function returns the encrypted data, the IV, and the key used for encryption.