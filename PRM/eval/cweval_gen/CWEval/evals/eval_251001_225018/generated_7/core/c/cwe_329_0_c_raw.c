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
    encryption_result result = {NULL, 0, NULL, NULL};
    if (key_len < AES_BLOCK_SIZE || data_len == 0) {
        return result;
    }

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result;
    }

    // Create an OpenSSL EVP_CIPHER_CTX object
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return result;
    }

    // Initialize the cipher context for encryption
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        return result;
    }

    // Allocate memory for the encrypted data
    size_t encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(encrypted_len);
    if (!result.encrypted_data) {
        EVP_CIPHER_CTX_free(ctx);
        return result;
    }

    // Encrypt the data
    int len;
    if (1 != EVP_EncryptUpdate(ctx, result.encrypted_data, &len, data, data_len)) {
        free(result.encrypted_data);
        EVP_CIPHER_CTX_free(ctx);
        return result;
    }
    result.encrypted_len = len;

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, result.encrypted_data + len, &len)) {
        free(result.encrypted_data);
        EVP_CIPHER_CTX_free(ctx);
        return result;
    }
    result.encrypted_len += len;

    // Copy the IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        EVP_CIPHER_CTX_free(ctx);
        return result;
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

    // Copy the key
    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        EVP_CIPHER_CTX_free(ctx);
        return result;
    }
    memcpy(result.key, key, key_len);

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return result;
}
```