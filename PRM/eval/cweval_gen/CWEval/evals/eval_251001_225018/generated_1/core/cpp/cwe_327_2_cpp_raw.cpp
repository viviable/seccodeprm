```cpp

using namespace std;

#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <vector>

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

encryption_result encrypt_data(
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& data
) {
    // Choose AES-256-CBC for encryption
    const int key_length = 32; // AES-256 uses a 256-bit key
    const int block_size = AES_BLOCK_SIZE;

    // Generate a random IV
    std::vector<unsigned char> iv(block_size);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the data to ensure it's a multiple of the block size
    std::vector<unsigned char> padded_data = pad_data(data);

    // Create an AES encryption context
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key.data(), key_length * 8, &enc_key) < 0) {
        throw std::runtime_error("Failed to set AES encryption key");
    }

    // Allocate memory for the encrypted data
    std::vector<unsigned char> encrypted_data(padded_data.size());

    // Encrypt the data
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv.data(), AES_ENCRYPT);

    // Return the result
    return {encrypted_data, iv, vector<unsigned char>(key.begin(), key.begin() + key_length)};
}
```