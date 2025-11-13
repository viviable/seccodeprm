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
    
    // Use AES-256-CBC as the chosen algorithm
    if (key_len < 32) {
        fprintf(stderr, "Key too short for AES-256\n");
        return result;
    }
    
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char* encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted_data) {
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    
    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        free(encrypted_data);
        fprintf(stderr, "Failed to generate random IV\n");
        return result;
    }
    
    // Initialize AES key
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        free(encrypted_data);
        fprintf(stderr, "Failed to set AES key\n");
        return result;
    }
    
    // Encrypt data
    int len;
    AES_cbc_encrypt(data, encrypted_data, data_len, &aes_key, iv, AES_ENCRYPT);
    
    result.encrypted_data = encrypted_data;
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.iv = malloc(sizeof(iv));
    if (!result.iv) {
        free(encrypted_data);
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    memcpy(result.iv, iv, sizeof(iv));
    result.key = malloc(32); // AES-256 key length
    if (!result.key) {
        free(encrypted_data);
        free(result.iv);
        fprintf(stderr, "Memory allocation failed\n");
        return result;
    }
    memcpy(result.key, key, 32);
    
    return result;
}
```