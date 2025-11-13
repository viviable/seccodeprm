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
    
    // Use AES-256-CBC as the encryption algorithm
    if (key_len >= 32) {
        AES_KEY enc_key;
        unsigned char iv[AES_BLOCK_SIZE];
        
        // Generate random IV
        if (!RAND_bytes(iv, sizeof(iv))) {
            goto cleanup;
        }
        
        // Initialize AES key
        if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
            goto cleanup;
        }
        
        // Calculate encrypted data length
        size_t encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
        result.encrypted_data = (unsigned char*)malloc(encrypted_len);
        if (!result.encrypted_data) {
            goto cleanup;
        }
        
        // Encrypt data
        AES_cbc_encrypt(data, result.encrypted_data, encrypted_len, &enc_key, iv, AES_ENCRYPT);
        
        // Store IV and key
        result.iv = (unsigned char*)malloc(sizeof(iv));
        if (!result.iv) {
            goto cleanup;
        }
        memcpy(result.iv, iv, sizeof(iv));
        
        result.key = (unsigned char*)malloc(key_len);
        if (!result.key) {
            goto cleanup;
        }
        memcpy(result.key, key, key_len);
        
        result.encrypted_len = encrypted_len;
    }
    
cleanup:
    // Free allocated memory on error
    free(result.encrypted_data);
    free(result.iv);
    free(result.key);
    
    return result;
}
```