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
    // Ensure the key length is at least 16 bytes for AES-128
    if (key_len < 16) {
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Determine the effective key length based on the provided key length
    size_t effective_key_len = key_len > 32 ? 32 : key_len; // AES-256 has a maximum key length of 32 bytes

    // Allocate memory for the encrypted data
    unsigned char* encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted_data) {
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Initialize the AES context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        free(encrypted_data);
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Set up the AES encryption
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        free(encrypted_data);
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Encrypt the data
    int len;
    if (1 != EVP_EncryptUpdate(ctx, encrypted_data, &len, data, data_len)) {
        EVP_CIPHER_CTX_free(ctx);
        free(encrypted_data);
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }
    int final_len;
    if (1 != EVP_EncryptFinal_ex(ctx, encrypted_data + len, &final_len)) {
        EVP_CIPHER_CTX_free(ctx);
        free(encrypted_data);
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }
    size_t total_len = len + final_len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Prepare the result structure
    encryption_result result = {
        .encrypted_data = encrypted_data,
        .encrypted_len = total_len,
        .iv = iv,
        .key = (unsigned char*)malloc(effective_key_len);
    };
    if (!result.key) {
        free(encrypted_data);
        memset(&result, 0, sizeof(result));
        return result;
    }
    memcpy(result.key, key, effective_key_len);

    return result;
}
```