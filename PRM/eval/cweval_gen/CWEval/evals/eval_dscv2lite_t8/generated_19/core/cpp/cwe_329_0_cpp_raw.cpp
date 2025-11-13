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

std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>> encrypt_data(
    const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {

    // Determine the key length based on OpenSSL's requirements
    int key_length = AES_BLOCK_SIZE; // AES-128
    if (key.size() >= 96 / 8) key_length = 192 / 8; // AES-192
    if (key.size() >= 128 / 8) key_length = 256 / 8; // AES-256

    // Create a subkey from the provided key
    std::vector<uint8_t> subkey(key_length);
    memcpy(subkey.data(), key.data(), key_length);

    // Generate a random IV
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw runtime_error("Failed to generate IV");
    }

    // Pad the data according to PKCS7
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Allocate space for the encrypted data
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Encrypt the data
    AES_KEY aes_key;
    if (AES_set_encrypt_key(subkey.data(), key_length * 8, &aes_key) != 0) {
        throw runtime_error("Failed to set AES encryption key");
    }
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &aes_key, iv.data(), AES_ENCRYPT);

    return make_tuple(encrypted_data, iv, subkey);
}
```