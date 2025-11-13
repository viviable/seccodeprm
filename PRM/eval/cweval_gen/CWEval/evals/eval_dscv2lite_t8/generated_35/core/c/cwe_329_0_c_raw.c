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
    encryption_result result;
    result.encrypted_data = NULL;
    result.encrypted_len = 0;
    result.iv = NULL;
    result.key = NULL;

    // Ensure the key length is at least 16 bytes (AES-128)
    if (key_len < 16) {
        return result;
    }

    // Allocate memory for the IV and the encrypted data
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        return result;
    }
    result.encrypted_data = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        free(result.iv);
        return result;
    }

    // Initialize the IV randomly
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        free(result.encrypted_data);
        free(result.iv);
        return result;
    }

    // Create an AES cipher context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        free(result.encrypted_data);
        free(result.iv);
        return result;
    }

    // Initialize the cipher context for encryption
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, result.iv)) {
        EVP_CIPHER_CTX_free(ctx);
        free(result.encrypted_data);
        free(result.iv);
        return result;
    }

    int len;
    // Encrypt the data
    if (1 != EVP_EncryptUpdate(ctx, result.encrypted_data, &len, data, data_len)) {
        EVP_CIPHER_CTX_free(ctx);
        free(result.encrypted_data);
        free(result.iv);
        return result;
    }
    result.encrypted_len += len;

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, result.encrypted_data + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        free(result.encrypted_data);
        free(result.iv);
        return result;
    }
    result.encrypted_len += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Duplicate the key for returning
    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result;
    }
    memcpy(result.key, key, key_len);

    return result;
}
```