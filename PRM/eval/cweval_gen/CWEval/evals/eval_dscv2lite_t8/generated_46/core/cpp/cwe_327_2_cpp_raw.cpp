```cpp

using namespace std;

encryption_result encrypt_data(
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& data
) {
    encryption_result result;

    // Use AES-256-CBC for encryption
    unsigned char aes_key[32];
    memcpy(aes_key, key.data(), 32);

    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        throw runtime_error("Failed to generate random IV");
    }

    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 256, &enc_key) != 0) {
        throw runtime_error("Failed to set AES encryption key");
    }

    size_t padded_len = pad_data(data).size();
    result.encrypted_data.resize(padded_len);
    AES_cbc_encrypt(pad_data(data).data(), result.encrypted_data.data(), padded_len, &enc_key, iv, AES_ENCRYPT);

    result.iv.assign(iv, iv + AES_BLOCK_SIZE);
    result.key.assign(aes_key, aes_key + 32);

    return result;
}
```