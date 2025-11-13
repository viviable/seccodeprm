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
    
    // Choose AES-256-CBC as the encryption algorithm
    if (key_len >= 32) {
        AES_KEY enc_key;
        unsigned char iv[AES_BLOCK_SIZE];
        
        // Generate a random IV
        if (!RAND_bytes(iv, sizeof(iv))) {
            goto cleanup;
        }
        
        // Set up the encryption key
        if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
            goto cleanup;
        }
        
        // Calculate the padded data length
        size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
        unsigned char* padded_data = malloc(padded_len);
        if (!padded_data) {
            goto cleanup;
        }
        memcpy(padded_data, data, data_len);
        memset(padded_data + data_len, 0, padded_len - data_len);
        
        // Allocate memory for the encrypted data
        result.encrypted_data = malloc(padded_len);
        if (!result.encrypted_data) {
            free(padded_data);
            goto cleanup;
        }
        
        // Perform the encryption
        AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, &enc_key, iv, AES_ENCRYPT);
        
        // Store the results
        result.encrypted_len = padded_len;
        result.iv = iv;
        result.key = (unsigned char*)malloc(32);
        if (!result.key) {
            free(result.encrypted_data);
            goto cleanup;
        }
        memcpy(result.key, key, 32);
        
        // Free temporary data
        free(padded_data);
        
        return result;
    }

cleanup:
    // Clean up allocated resources
    if (result.encrypted_data) free(result.encrypted_data);
    if (result.iv) free(result.iv);
    if (result.key) free(result.key);
    
    return result;
}
```