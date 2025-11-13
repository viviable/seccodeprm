```cpp

using namespace std;

encryption_result encrypt_data(
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& data
) {
    encryption_result result;

    // Use AES-256-CBC for encryption
    unsigned char aes_key[AES_BLOCK_SIZE * 2];
    memcpy(aes_key, key.data(), AES_BLOCK_SIZE * 2);

    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        throw runtime_error("Failed to generate IV");
    }

    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 256, &enc_key) != 0) {
        throw runtime_error("Failed to set AES encryption key");
    }

    vector<unsigned char> padded_data = pad_data(data);
    result.encrypted_data.resize(padded_data.size());

    AES_cbc_encrypt(padded_data.data(), result.encrypted_data.data(), padded_data.size(), &enc_key, iv, AES_ENCRYPT);

    result.iv.assign(iv, iv + AES_BLOCK_SIZE);
    result.key.assign(key.begin(), key.begin() + AES_BLOCK_SIZE * 2);

    return result;
}
```