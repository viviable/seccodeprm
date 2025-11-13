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
    
    // Ensure key length is valid for AES
    if (key_len < AES_BLOCK_SIZE || key_len > 32 * AES_BLOCK_SIZE) {
        return result;
    }
    
    // Select the appropriate AES key length
    AES_KEY aes_key;
    int key_bits = key_len * 8;
    if (AES_set_encrypt_key(key, key_bits, &aes_key) != 0) {
        return result;
    }
    
    // Generate a random IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        return result;
    }
    RAND_bytes(result.iv, AES_BLOCK_SIZE);
    
    // Calculate the encrypted data length
    size_t encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(encrypted_len);
    if (!result.encrypted_data) {
        free(result.iv);
        return result;
    }
    
    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, result.iv, AES_ENCRYPT);
    result.encrypted_len = encrypted_len;
    
    // Copy the key
    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        free(result.iv);
        free(result.encrypted_data);
        return result;
    }
    memcpy(result.key, key, key_len);
    
    return result;
}
```