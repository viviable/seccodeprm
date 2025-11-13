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
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Error generating IV
    }
    
    // Use AES-256-CBC for encryption
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
        return result; // Error setting up encryption key
    }
    
    // Calculate the total length needed for padding
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* padded_data = (unsigned char*)malloc(padded_len);
    if (!padded_data) {
        return result; // Memory allocation failed
    }
    memcpy(padded_data, data, data_len);
    // Simple PKCS#7 padding
    for (size_t i = data_len; i < padded_len; ++i) {
        padded_data[i] = padded_len - data_len;
    }
    
    // Allocate memory for the encrypted data
    result.encrypted_data = (unsigned char*)malloc(padded_len);
    if (!result.encrypted_data) {
        free(padded_data);
        return result; // Memory allocation failed
    }
    
    // Perform the encryption
    AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, &enc_key, iv, AES_ENCRYPT);
    
    // Free temporary buffers
    free(padded_data);
    
    // Store the IV and key
    result.iv = (unsigned char*)malloc(sizeof(iv));
    if (!result.iv) {
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.iv, iv, sizeof(iv));
    
    result.key = (unsigned char*)malloc(32); // AES-256 key length
    if (!result.key) {
        free(result.iv);
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, 32);
    
    result.encrypted_len = padded_len;
    
    return result;
}
```