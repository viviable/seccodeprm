```c
#include <openssl/aes.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned char* encrypted_data;
    size_t encrypted_len;
    unsigned char* iv;
    unsigned char* key;
} encryption_result;

encryption_result encrypt_data(
    const unsigned char* key, size_t key_len,
    const unsigned char* data, size_t data_len
) {
    // Determine the effective key length for AES (128, 192, or 256 bits)
    size_t effective_key_len = key_len <= 32 ? key_len : 32; // 128-bit
    if (key_len > 32 && key_len <= 48) {
        effective_key_len = 48; // 192-bit
    } else if (key_len > 48) {
        effective_key_len = 64; // 256-bit
    }

    // Allocate memory for the IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        // Handle error: failed to generate random IV
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the encrypted data
    size_t encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* encrypted_data = malloc(encrypted_len);
    if (!encrypted_data) {
        // Handle error: failed to allocate memory for encrypted data
        exit(EXIT_FAILURE);
    }

    // Create an AES cipher context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        // Handle error: failed to create cipher context
        free(encrypted_data);
        exit(EXIT_FAILURE);
    }

    // Initialize the cipher context for encryption
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        // Handle error: failed to initialize cipher context
        EVP_CIPHER_CTX_free(ctx);
        free(encrypted_data);
        exit(EXIT_FAILURE);
    }

    // Encrypt the data
    int len;
    if (1 != EVP_EncryptUpdate(ctx, encrypted_data, &len, data, data_len)) {
        // Handle error: failed to encrypt data
        EVP_CIPHER_CTX_free(ctx);
        free(encrypted_data);
        exit(EXIT_FAILURE);
    }
    size_t written_len = len;

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, encrypted_data + written_len, &len)) {
        // Handle error: failed to finalize encryption
        EVP_CIPHER_CTX_free(ctx);
        free(encrypted_data);
        exit(EXIT_FAILURE);
    }
    written_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Prepare the result structure
    encryption_result result;
    result.encrypted_data = encrypted_data;
    result.encrypted_len = written_len;
    result.iv = iv;
    result.key = (unsigned char*)malloc(effective_key_len);
    memcpy(result.key, key, effective_key_len);

    return result;
}
```