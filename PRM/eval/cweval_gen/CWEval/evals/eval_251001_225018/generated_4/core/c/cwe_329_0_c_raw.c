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
    // Ensure the key length is at least 32 bytes for AES-256
    if (key_len < 32) {
        fprintf(stderr, "Key must be at least 32 bytes for AES-256\n");
        exit(1);
    }

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        fprintf(stderr, "Failed to generate IV\n");
        exit(1);
    }

    // Create an AES cipher context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "Failed to create cipher context\n");
        exit(1);
    }

    // Initialize the cipher context for encryption
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        fprintf(stderr, "Failed to initialize encryption\n");
        EVP_CIPHER_CTX_free(ctx);
        exit(1);
    }

    // Determine the maximum buffer size needed for encryption
    size_t max_encrypted_len = data_len + AES_BLOCK_SIZE;
    unsigned char* encrypted_data = malloc(max_encrypted_len);
    if (!encrypted_data) {
        fprintf(stderr, "Memory allocation failed\n");
        EVP_CIPHER_CTX_free(ctx);
        exit(1);
    }

    int len;
    // Perform the encryption
    if (1 != EVP_EncryptUpdate(ctx, encrypted_data, &len, data, data_len)) {
        fprintf(stderr, "Encryption failed\n");
        free(encrypted_data);
        EVP_CIPHER_CTX_free(ctx);
        exit(1);
    }
    size_t encrypted_len = len;

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, encrypted_data + len, &len)) {
        fprintf(stderr, "Final encryption failed\n");
        free(encrypted_data);
        EVP_CIPHER_CTX_free(ctx);
        exit(1);
    }
    encrypted_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Return the result
    encryption_result result;
    result.encrypted_data = encrypted_data;
    result.encrypted_len = encrypted_len;
    result.iv = iv;
    result.key = (unsigned char*)malloc(32); // Allocate memory for the key
    if (!result.key) {
        fprintf(stderr, "Memory allocation failed for key\n");
        free(result.encrypted_data);
        exit(1);
    }
    memcpy(result.key, key, 32);

    return result;
}
```