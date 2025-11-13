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
    // Choose the appropriate AES key length based on the provided key length
    AES_KEY aes_key;
    int aes_key_length = key_len <= 32 ? AES_128 : key_len <= 48 ? AES_192 : AES_256;
    
    // Allocate memory for the result structure
    encryption_result result;
    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    result.iv = malloc(AES_BLOCK_SIZE);
    result.key = malloc(key_len);
    
    if (!result.encrypted_data || !result.iv || !result.key) {
        // Handle memory allocation failure
        if (result.encrypted_data) free(result.encrypted_data);
        if (result.iv) free(result.iv);
        if (result.key) free(result.key);
        memset(&result, 0, sizeof(encryption_result));
        return result;
    }
    
    // Copy the key into the result structure
    memcpy(result.key, key, key_len);
    
    // Generate a random IV
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        // Handle error in generating random IV
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        memset(&result, 0, sizeof(encryption_result));
        return result;
    }
    
    // Initialize the AES key
    if (AES_set_encrypt_key(key, aes_key_length * 8, &aes_key) != 0) {
        // Handle error in setting up AES key
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        memset(&result, 0, sizeof(encryption_result));
        return result;
    }
    
    // Pad the data to make it a multiple of the block size
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* padded_data = malloc(padded_len);
    if (!padded_data) {
        // Handle memory allocation failure for padding
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        memset(&result, 0, sizeof(encryption_result));
        return result;
    }
    memcpy(padded_data, data, data_len);
    for (size_t i = data_len; i < padded_len; ++i) {
        padded_data[i] = 0x00; // Simple zero padding
    }
    
    // Encrypt the data
    AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, &aes_key, result.iv, AES_ENCRYPT);
    
    // Clean up
    free(padded_data);
    result.encrypted_len = padded_len;
    
    return result;
}
```