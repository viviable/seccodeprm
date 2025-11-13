```cpp

using namespace std;

encryption_result encrypt_data(
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& data
) {
    // Select AES-256-CBC for encryption
    unsigned char aes_key[32];
    memcpy(aes_key, key.data(), 32);

    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        throw runtime_error("Failed to generate IV");
    }

    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 256, &enc_key) != 0) {
        throw runtime_error("Failed to set encryption key");
    }

    std::vector<unsigned char> padded_data = pad_data(data);
    size_t padded_len = padded_data.size();

    std::vector<unsigned char> encrypted_data(padded_len);
    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_len, &enc_key, iv, AES_ENCRYPT);

    encryption_result result;
    result.encrypted_data = encrypted_data;
    result.iv = iv;
    result.key.assign(aes_key, aes_key + 32);

    return result;
}
```