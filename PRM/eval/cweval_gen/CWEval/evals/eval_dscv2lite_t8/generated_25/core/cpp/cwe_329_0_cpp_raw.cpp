```cpp

using namespace std;

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <vector>
#include <tuple>

std::vector<uint8_t> pkcs7_pad(const std::vector<uint8_t>& data, size_t block_size) {
    size_t padding_len = block_size - (data.size() % block_size);
    std::vector<uint8_t> padded = data;
    padded.insert(padded.end(), padding_len, static_cast<uint8_t>(padding_len));
    return padded;
}

std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>>
encrypt_data(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    // Ensure the key is at least 32 bytes (AES-256)
    if (key.size() < 32) {
        throw std::invalid_argument("Key must be at least 32 bytes long");
    }

    // Create an IV and ensure it's 16 bytes long (AES block size)
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the data to make it a multiple of the block size
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Create a buffer for the encrypted data
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Encrypt the data using AES-CBC mode
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key.data(), 256, &enc_key) != 0) {
        throw std::runtime_error("Failed to set up AES encryption key");
    }
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv.data(), AES_ENCRYPT);

    return {encrypted_data, iv, key};
}
```
This code snippet includes the necessary OpenSSL headers and implements the `encrypt_data` function, which encrypts the provided data using AES-256 in CBC mode with a randomly generated IV. It also pads the data according to PKCS7 standards before encryption.