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
    // Determine the key length based on OpenSSL requirements
    int aes_key_len = 0;
    if (key_len >= 32) aes_key_len = AES_256_KEY_SIZE;
    else if (key_len >= 24) aes_key_len = AES_192_KEY_SIZE;
    else if (key_len >= 16) aes_key_len = AES_128_KEY_SIZE;
    else {
        // Handle error: key too short
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        // Handle error: could not generate IV
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Allocate memory for the encrypted data
    encryption_result result;
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        // Handle error: could not allocate memory
        result.iv = NULL;
        result.key = NULL;
        return result;
    }

    // Create an AES cipher context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        // Handle error: could not create cipher context
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }

    // Initialize the cipher context
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        // Handle error: could not initialize cipher
        EVP_CIPHER_CTX_free(ctx);
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }

    // Encrypt the data
    int len;
    if (1 != EVP_EncryptUpdate(ctx, result.encrypted_data, &len, data, data_len)) {
        // Handle error: could not update cipher
        EVP_CIPHER_CTX_free(ctx);
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }
    size_t written = len;

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, result.encrypted_data + written, &len)) {
        // Handle error: could not finalize cipher
        EVP_CIPHER_CTX_free(ctx);
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }
    written += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Assign IV and key to result
    result.iv = malloc(AES_BLOCK_SIZE);
    if (result.iv) {
        memcpy(result.iv, iv, AES_BLOCK_SIZE);
    } else {
        // Handle error: could not allocate memory for IV
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }

    result.key = malloc(aes_key_len);
    if (result.key) {
        memcpy(result.key, key, aes_key_len);
    } else {
        // Handle error: could not allocate memory for key
        free(result.encrypted_data);
        free(result.iv);
        result.encrypted_data = NULL;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }

    // Set the length of the encrypted data
    result.encrypted_len = written;

    return result;
}
```