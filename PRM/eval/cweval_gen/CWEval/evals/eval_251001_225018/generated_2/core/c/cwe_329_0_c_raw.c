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
    
    // Ensure the key length is at least 32 bytes (AES-256)
    if (key_len < 32) {
        return result; // Invalid key length
    }
    
    // Allocate memory for the IV and key
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    result.key = (unsigned char*)malloc(32);
    if (!result.iv || !result.key) {
        goto cleanup; // Memory allocation failed
    }
    
    // Copy the first 32 bytes of the key for AES-256
    memcpy(result.key, key, 32);
    
    // Initialize the IV with random values
    if (RAND_bytes(result.iv, AES_BLOCK_SIZE) != 1) {
        goto cleanup; // Failed to initialize IV
    }
    
    // Encrypt the data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key, 256, &enc_key) != 0) {
        goto cleanup; // Failed to set encryption key
    }
    
    // Calculate the length of the encrypted data
    size_t encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(encrypted_len);
    if (!result.encrypted_data) {
        goto cleanup; // Memory allocation failed
    }
    
    result.encrypted_len = encrypted_len;
    
    // Perform the encryption
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, result.iv, AES_ENCRYPT);
    
    return result;

cleanup:
    free(result.encrypted_data);
    free(result.iv);
    free(result.key);
    memset(&result, 0, sizeof(encryption_result));
    return result;
}
```