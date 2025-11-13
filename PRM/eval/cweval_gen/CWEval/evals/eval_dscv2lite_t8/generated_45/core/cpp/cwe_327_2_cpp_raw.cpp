```cpp

using namespace std;

encryption_result encrypt_data(
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& data
) {
    // Choose AES-256-CBC for this example
    const unsigned int key_length = 32; // AES-256
    const unsigned int iv_length = AES_BLOCK_SIZE;

    // Ensure the key is at least as long as the chosen algorithm requires
    if (key.size() < key_length) {
        throw runtime_error("Key must be at least 32 bytes long for AES-256");
    }

    // Generate a random IV
    std::vector<unsigned char> iv(iv_length);
    if (!RAND_bytes(iv.data(), iv_length)) {
        throw runtime_error("Failed to generate random IV");
    }

    // Pad the data to ensure it's a multiple of the block size
    std::vector<unsigned char> padded_data = pad_data(data);

    // Create the result structure
    encryption_result result;

    // Encrypt the data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key.data(), key_length * 8, &enc_key) < 0) {
        throw runtime_error("Failed to set up AES encryption key");
    }

    result.encrypted_data.resize(padded_data.size());
    AES_cbc_encrypt(padded_data.data(), result.encrypted_data.data(), padded_data.size(), &enc_key, iv.data(), AES_ENCRYPT);

    // Store the IV and key
    result.iv = iv;
    result.key.assign(key.begin(), key.begin() + key_length);

    return result;
}
```