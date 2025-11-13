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
    // Determine the key size based on OpenSSL's AES requirements
    int key_length = AES_BLOCK_SIZE; // 128 bits for AES-128
    if (key.size() >= 32 * AES_BLOCK_SIZE) {
        key_length = 32 * AES_BLOCK_SIZE; // 256 bits for AES-256
    } else if (key.size() >= 24 * AES_BLOCK_SIZE) {
        key_length = 24 * AES_BLOCK_SIZE; // 192 bits for AES-192
    }

    // Create a sub-key from the provided key
    std::vector<uint8_t> sub_key(key.begin(), key.begin() + key_length);

    // Generate a random initialization vector (IV)
    std::vector<uint8_t> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw runtime_error("Failed to generate IV");
    }

    // Pad the data to make it a multiple of the block size
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Allocate space for the encrypted data
    std::vector<uint8_t> encrypted_data(padded_data.size());

    // Initialize the AES context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw runtime_error("Failed to create AES context");
    }
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, sub_key.data(), iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        throw runtime_error("Encryption initialization failed");
    }

    // Encrypt the data
    int len;
    if (1 != EVP_EncryptUpdate(ctx, encrypted_data.data(), &len, padded_data.data(), padded_data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw runtime_error("Encryption update failed");
    }
    int ciphertext_len = len;

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, encrypted_data.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw runtime_error("Encryption finalization failed");
    }
    ciphertext_len += len;

    // Resize the encrypted data vector to the actual length
    encrypted_data.resize(ciphertext_len);

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Return the results as a tuple
    return std::make_tuple(encrypted_data, iv, sub_key);
}
```