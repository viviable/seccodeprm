```cpp

using namespace std;

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
    encryption_result result;
    std::vector<unsigned char> padded_data = pad_data(data);
    std::vector<unsigned char> iv(AES_BLOCK_SIZE);
    std::vector<unsigned char> aes_key(32); // Using AES-256

    // Generate random IV
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw runtime_error("Failed to generate IV");
    }

    // Use the first 32 bytes of the key for AES-256
    memcpy(aes_key.data(), key.data(), aes_key.size());

    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key.data(), 256, &enc_key) != 0) {
        throw runtime_error("Failed to set AES encryption key");
    }

    // Encrypt data
    result.encrypted_data.resize(padded_data.size());
    AES_cbc_encrypt(padded_data.data(), result.encrypted_data.data(),
                    padded_data.size(), &enc_key, iv.data(), AES_ENCRYPT);

    result.iv = iv;
    result.key = aes_key;

    return result;
}
```