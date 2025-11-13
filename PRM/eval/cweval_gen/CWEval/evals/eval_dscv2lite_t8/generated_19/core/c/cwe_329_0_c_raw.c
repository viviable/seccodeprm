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
    unsigned char iv[AES_BLOCK_SIZE];
    AES_KEY enc_key;
    
    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }
    
    // Ensure key length is appropriate for AES-256
    size_t effective_key_len = key_len > 32 ? 32 : key_len;
    
    // Set up encryption key
    if (AES_set_encrypt_key(key, effective_key_len * 8, &enc_key) != 0) {
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }
    
    // Calculate the padded data length
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* padded_data = (unsigned char*)malloc(padded_len);
    if (!padded_data) {
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }
    
    // Pad the data
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, 0, padded_len - data_len);
    
    // Allocate memory for encrypted data
    result.encrypted_data = (unsigned char*)malloc(padded_len);
    if (!result.encrypted_data) {
        free(padded_data);
        result.encrypted_len = 0;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }
    
    // Encrypt the data
    AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, &enc_key, iv, AES_ENCRYPT);
    
    // Free temporary buffers
    free(padded_data);
    
    // Store the results
    result.encrypted_len = padded_len;
    result.iv = (unsigned char*)malloc(sizeof(iv));
    if (!result.iv) {
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.key = NULL;
        return result;
    }
    memcpy(result.iv, iv, sizeof(iv));
    
    result.key = (unsigned char*)malloc(effective_key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.iv = NULL;
        return result;
    }
    memcpy(result.key, key, effective_key_len);
    
    return result;
}
```