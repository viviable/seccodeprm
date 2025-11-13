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
    // Determine the appropriate AES key size based on the provided key length
    int aes_key_length = (key.size() >= 32) ? AES_KEY_LENGTH_256 : 
                          (key.size() >= 16) ? AES_KEY_LENGTH_192 : AES_KEY_LENGTH_128;

    // Create an AES key structure
    AES_KEY enc_key;
    if (aes_key_length == AES_KEY_LENGTH_256) {
        if (AES_set_encrypt_key(key.data(), 256, &enc_key) != 0) {
            throw std::runtime_error("Failed to set AES encryption key");
        }
    } else if (aes_key_length == AES_KEY_LENGTH_192) {
        if (AES_set_encrypt_key(key.data(), 192, &enc_key) != 0) {
            throw std::runtime_error("Failed to set AES encryption key");
        }
    } else {
        if (AES_set_encrypt_key(key.data(), 128, &enc_key) != 0) {
            throw std::runtime_error("Failed to set AES encryption key");
        }
    }

    // Generate a random initialization vector (IV)
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the input data according to PKCS7
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Allocate memory for the encrypted data
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Encrypt the data using AES-CBC mode
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv.data(), AES_ENCRYPT);

    // Return the encrypted data, IV, and key used for encryption
    return std::make_tuple(encrypted_data, iv, std::vector<uint8_t>(key.begin(), key.begin() + aes_key_length));
}
```