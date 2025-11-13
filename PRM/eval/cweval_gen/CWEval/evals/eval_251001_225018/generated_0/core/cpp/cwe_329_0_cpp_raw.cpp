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

std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>> encrypt_data(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    size_t block_size = AES_BLOCK_SIZE;
    if (key.size() < block_size * 2) {
        throw std::invalid_argument("Key must be at least twice the block size");
    }

    // Extract the key and IV from the provided key bytes
    std::vector<uint8_t> enc_key(key.begin(), key.begin() + block_size);
    std::vector<uint8_t> iv(key.begin() + block_size, key.begin() + block_size * 2);

    // Pad the data to make it a multiple of the block size
    std::vector<uint8_t> padded_data = pkcs7_pad(data, block_size);

    // Encrypt the data
    AES_KEY aes_key;
    AES_set_encrypt_key(enc_key.data(), block_size * 8, &aes_key);

    std::vector<uint8_t> encrypted_data(padded_data.size());
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &aes_key, iv.data(), AES_ENCRYPT);

    return std::make_tuple(encrypted_data, iv, enc_key);
}
```