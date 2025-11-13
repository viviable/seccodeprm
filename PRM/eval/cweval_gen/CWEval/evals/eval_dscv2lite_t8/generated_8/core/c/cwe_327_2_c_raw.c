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
    
    // Use AES-256-CBC as the encryption algorithm
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
        return result; // Key setup failed
    }
    
    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // IV generation failed
    }
    
    // Calculate the total encrypted length
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(padded_len);
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    result.key = (unsigned char*)malloc(32); // AES-256 key length
    
    if (!result.encrypted_data || !result.iv || !result.key) {
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        return result; // Memory allocation failed
    }
    
    memcpy(result.iv, iv, AES_BLOCK_SIZE);
    memcpy(result.key, key, 32);
    
    // Pad the data
    unsigned char* padded_data = (unsigned char*)malloc(data_len + 1);
    if (!padded_data) {
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        return result; // Memory allocation failed
    }
    memcpy(padded_data, data, data_len);
    padded_data[data_len] = 0; // Null terminator for simplicity
    
    // Encrypt the data
    AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, &enc_key, iv, AES_ENCRYPT);
    
    // Clean up
    free(padded_data);
    
    result.encrypted_len = padded_len;
    
    return result;
}
```