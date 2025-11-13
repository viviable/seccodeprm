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
    encryption_result result;
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char* padded_data = nullptr;
    int padded_len = 0;

    // Pad the data to the AES block size
    padded_data = new unsigned char[data.size()];
    padded_len = pad_data(data).size();

    // Generate random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        delete[] padded_data;
        throw runtime_error("Failed to generate IV");
    }

    // Set up AES key
    if (AES_set_encrypt_key(&key[0], 256, &enc_key) < 0) {
        delete[] padded_data;
        throw runtime_error("Failed to set AES key");
    }

    // Encrypt the data
    result.encrypted_data.resize(padded_len);
    AES_cbc_encrypt(padded_data, &result.encrypted_data[0], padded_len, &enc_key, iv, AES_ENCRYPT);

    // Clean up
    delete[] padded_data;

    // Store the IV and key
    result.iv.assign(iv, iv + AES_BLOCK_SIZE);
    result.key.assign(key.begin(), key.begin() + 32); // Assuming 256-bit key

    return result;
}
```