```cpp

using namespace std;

encryption_result encrypt_data(
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& data
) {
    // Choose AES-256-CBC as the encryption algorithm
    const int key_length = 32; // AES-256 requires a 32-byte key
    const int iv_length = AES_BLOCK_SIZE; // AES block size is 16 bytes

    unsigned char aes_key[key_length];
    memcpy(aes_key, key.data(), key_length);

    unsigned char iv[iv_length];
    if (!RAND_bytes(iv, iv_length)) {
        throw runtime_error("Failed to generate IV");
    }

    std::vector<unsigned char> padded_data = pad_data(data);
    std::vector<unsigned char> encrypted_data(padded_data.size());

    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 8 * key_length, &enc_key) != 0) {
        throw runtime_error("Failed to set encryption key");
    }

    AES_cbc_encrypt(padded_data.data(), encrypted_data.data(), padded_data.size(), &enc_key, iv, AES_ENCRYPT);

    encryption_result result;
    result.encrypted_data = encrypted_data;
    result.iv = iv;
    result.key.assign(aes_key, aes_key + key_length);

    return result;
}
```