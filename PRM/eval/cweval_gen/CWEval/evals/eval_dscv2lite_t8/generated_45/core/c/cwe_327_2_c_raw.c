```c
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <stdlib.h>

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
    
    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        goto cleanup;
    }
    
    // Use the first 32 bytes of the key as the AES key
    unsigned char aes_key[32];
    memcpy(aes_key, key, sizeof(aes_key));
    
    // Create an AES context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        goto cleanup;
    }
    
    // Initialize the AES context for encryption
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, iv)) {
        goto cleanup;
    }
    
    // Determine the buffer size needed for encryption
    int ciphertext_len = 0;
    if (1 != EVP_EncryptUpdate(ctx, NULL, &ciphertext_len, data, data_len)) {
        goto cleanup;
    }
    
    // Allocate memory for the ciphertext
    result.encrypted_data = malloc(ciphertext_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        goto cleanup;
    }
    
    // Encrypt the data
    if (1 != EVP_EncryptUpdate(ctx, result.encrypted_data, &ciphertext_len, data, data_len)) {
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        goto cleanup;
    }
    
    // Finalize the encryption
    int final_len = 0;
    if (1 != EVP_EncryptFinal_ex(ctx, result.encrypted_data + ciphertext_len, &final_len)) {
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        goto cleanup;
    }
    
    // Update the total ciphertext length
    result.encrypted_len = ciphertext_len + final_len;
    
    // Copy the IV and key
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        goto cleanup;
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);
    
    result.key = malloc(32);
    if (!result.key) {
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        free(result.iv);
        result.iv = NULL;
        goto cleanup;
    }
    memcpy(result.key, aes_key, 32);
    
cleanup:
    EVP_CIPHER_CTX_free(ctx);
    return result;
}
```