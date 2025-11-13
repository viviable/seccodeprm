```c
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
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
    
    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        goto end;
    }
    
    // Use the first 32 bytes of the key for AES-256
    unsigned char aes_key[32];
    memset(aes_key, 0, sizeof(aes_key));
    memcpy(aes_key, key, sizeof(aes_key));
    
    // Allocate memory for the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        goto end;
    }
    
    // Encrypt the data using AES-256-CBC
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        goto free_encrypted_data;
    }
    
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, aes_key, iv)) {
        goto free_ctx;
    }
    
    int len;
    if (1 != EVP_EncryptUpdate(ctx, result.encrypted_data, &len, data, data_len)) {
        goto free_ctx;
    }
    int final_len;
    if (1 != EVP_EncryptFinal_ex(ctx, result.encrypted_data + len, &final_len)) {
        goto free_ctx;
    }
    result.encrypted_len = len + final_len;
    
    // Save the IV and key
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        goto free_encrypted_data;
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);
    
    result.key = malloc(32);
    if (!result.key) {
        goto free_iv;
    }
    memcpy(result.key, aes_key, 32);
    
    EVP_CIPHER_CTX_free(ctx);
    return result;

free_iv:
    free(result.iv);
free_encrypted_data:
    free(result.encrypted_data);
free_ctx:
    EVP_CIPHER_CTX_free(ctx);
end:
    return result;
}
```