```c
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>

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
    unsigned char aes_key[32];
    unsigned char des_key[8];
    unsigned char iv[AES_BLOCK_SIZE];

    // Generate a random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        goto cleanup;
    }

    // Copy the appropriate portion of the key to the respective encryption key
    if (key_len >= 32) {
        memcpy(aes_key, key, 32);
    } else {
        memset(aes_key, 0, 32);
        memcpy(aes_key, key, key_len);
    }
    if (key_len >= 8) {
        memcpy(des_key, key + 32, 8);
    } else {
        memset(des_key, 0, 8);
        memcpy(des_key, key, key_len - 32);
    }

    // Encrypt the data using AES-256-CBC
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        goto cleanup;
    }
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        goto cleanup;
    }
    int len;
    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        EVP_CIPHER_CTX_free(ctx);
        goto cleanup;
    }
    if (1 != EVP_EncryptUpdate(ctx, result.encrypted_data, &len, data, data_len)) {
        free(result.encrypted_data);
        EVP_CIPHER_CTX_free(ctx);
        goto cleanup;
    }
    int final_len;
    if (1 != EVP_EncryptFinal_ex(ctx, result.encrypted_data + len, &final_len)) {
        free(result.encrypted_data);
        EVP_CIPHER_CTX_free(ctx);
        goto cleanup;
    }
    result.encrypted_len = len + final_len;
    EVP_CIPHER_CTX_free(ctx);

    // Store the key and IV
    result.key = malloc(32);
    if (!result.key) {
        free(result.encrypted_data);
        goto cleanup;
    }
    memcpy(result.key, aes_key, 32);
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        free(result.key);
        goto cleanup;
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

cleanup:
    return result;
}
```