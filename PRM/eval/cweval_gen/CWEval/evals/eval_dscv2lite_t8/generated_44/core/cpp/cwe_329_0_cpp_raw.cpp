```cpp

using namespace std;

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <vector>

// Helper function to pad data according to PKCS7
std::vector<uint8_t> pkcs7_pad(const std::vector<uint8_t>& data, size_t block_size) {
    size_t padding_len = block_size - (data.size() % block_size);
    std::vector<uint8_t> padded = data;
    padded.insert(padded.end(), padding_len, static_cast<uint8_t>(padding_len));
    return padded;
}

std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>>
encrypt_data(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    // Ensure the key is at least 32 bytes (256 bits)
    if (key.size() < 32) {
        throw std::invalid_argument("Key must be at least 32 bytes");
    }

    // Get the first 32 bytes for the AES-256 key
    std::vector<uint8_t> aes_key(key.begin(), key.begin() + 32);

    // Generate a random IV
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the data according to PKCS7
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Encrypt the data
    std::vector<uint8_t> encrypted_data(padded_data.size());
    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key.data(), 256, &enc_key) != 0) {
        throw std::runtime_error("Failed to set encryption key");
    }
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv.data(), AES_ENCRYPT);

    return make_tuple(encrypted_data, iv, aes_key);
}
```
This implementation includes the necessary OpenSSL headers, defines a helper function for PKCS7 padding, and provides the `encrypt_data` function which encrypts the input data using AES-256 in CBC mode with a randomly generated IV. It also ensures that the key is at least 32 bytes long.