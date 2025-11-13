```cpp

using namespace std;

#include <openssl/aes.h>
#include <openssl/rand.h>
#include <vector>

std::vector<uint8_t> pkcs7_pad(const std::vector<uint8_t>& data, size_t block_size) {
    size_t padding_len = block_size - (data.size() % block_size);
    std::vector<uint8_t> padded = data;
    padded.insert(padded.end(), padding_len, static_cast<uint8_t>(padding_len));
    return padded;
}

std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>> encrypt_data(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    // Ensure the key length is at least 32 bytes for AES-256
    size_t key_length = std::min<size_t>(key.size(), 32);
    unsigned char aes_key[AES_BLOCK_SIZE];
    memcpy(aes_key, key.data(), key_length);

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Pad the data to be multiple of block size
    std::vector<uint8_t> padded_data = pkcs7_pad(data, AES_BLOCK_SIZE);

    // Encrypt the data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, key_length * 8, &enc_key) != 0) {
        throw std::runtime_error("Failed to set up AES encryption key");
    }

    std::vector<uint8_t> encrypted_data(padded_data.size());
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv, AES_ENCRYPT);

    return std::make_tuple(encrypted_data, std::vector<uint8_t>(iv, iv + AES_BLOCK_SIZE), std::vector<uint8_t>(aes_key, aes_key + key_length));
}
```