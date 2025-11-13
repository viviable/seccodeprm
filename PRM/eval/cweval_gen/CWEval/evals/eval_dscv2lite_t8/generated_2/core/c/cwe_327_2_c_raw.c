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
    
    // Use AES-256-CBC for encryption as it provides strong security
    if (key_len >= 32) {
        AES_KEY enc_key;
        unsigned char iv[AES_BLOCK_SIZE];
        
        // Generate random IV
        if (!RAND_bytes(iv, sizeof(iv))) {
            goto cleanup;
        }
        
        // Set up AES key
        if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
            goto cleanup;
        }
        
        // Calculate the length of the encrypted data
        size_t encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
        result.encrypted_data = (unsigned char*)malloc(encrypted_len);
        result.encrypted_len = encrypted_len;
        
        if (result.encrypted_data == NULL) {
            goto cleanup;
        }
        
        // Encrypt the data
        AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);
        
        // Copy the IV and key
        result.iv = (unsigned char*)malloc(sizeof(iv));
        result.key = (unsigned char*)malloc(32);
        
        if (result.iv == NULL || result.key == NULL) {
            free(result.encrypted_data);
            goto cleanup;
        }
        
        memcpy(result.iv, iv, sizeof(iv));
        memcpy(result.key, key, 32);
    } else {
        // Handle error: key is too short
        fprintf(stderr, "Key is too short\n");
    }

cleanup:
    return result;
}
```