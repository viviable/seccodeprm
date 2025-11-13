```cpp

using namespace std;

encryption_result encrypt_data(
    const std::vector<unsigned char>& key,
    const std::vector<unsigned char>& data
) {
    // Ensure the key length is at least 32 bytes for AES-256
    if (key.size() < 32) {
        throw std::invalid_argument("Key must be at least 32 bytes for AES-256");
    }

    // Pad the data to make it a multiple of AES block size
    std::vector<unsigned char> padded_data = pad_data(data);

    // Generate a random IV
    std::vector<unsigned char> iv(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), iv.size())) {
        throw std::runtime_error("Failed to generate random IV");
    }

    // Create an encryption result structure
    encryption_result result;

    // Initialize AES context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create AES context");
    }

    // Set up AES-256-CBC encryption
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize AES encryption");
    }

    // Encrypt the data
    int len;
    if (1 != EVP_EncryptUpdate(ctx, result.encrypted_data.data(), &len, padded_data.data(), padded_data.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to encrypt data");
    }
    result.encrypted_data.resize(len);

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, result.encrypted_data.data() + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize encryption");
    }
    result.encrypted_data.resize(len + result.encrypted_data.size());

    // Store the IV and key
    result.iv = iv;
    result.key = {key.begin(), key.begin() + 32};

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return result;
}
```