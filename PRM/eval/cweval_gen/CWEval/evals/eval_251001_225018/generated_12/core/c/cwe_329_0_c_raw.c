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
    if (key_len < AES_BLOCK_SIZE || data_len == 0) {
        return result; // Invalid input
    }

    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char* encrypted = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted) {
        return result; // Memory allocation failed
    }

    // Generate a random IV
    if (RAND_bytes(iv, AES_BLOCK_SIZE) != 1) {
        free(encrypted);
        return result; // Failed to generate IV
    }

    // Set up encryption key
    if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
        free(encrypted);
        return result; // Key setup failed
    }

    // Encrypt data
    int blocks = (data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;
    for (int i = 0; i < blocks; ++i) {
        AES_cbc_encrypt(data + i * AES_BLOCK_SIZE, encrypted + i * AES_BLOCK_SIZE, AES_BLOCK_SIZE, &enc_key, iv, AES_ENCRYPT);
        memcpy(iv, encrypted + i * AES_BLOCK_SIZE, AES_BLOCK_SIZE); // Update IV for next block
    }

    result.encrypted_data = encrypted;
    result.encrypted_len = data_len;
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    memcpy(result.iv, iv, AES_BLOCK_SIZE);
    result.key = (unsigned char*)malloc(key_len);
    memcpy(result.key, key, key_len);

    return result;
}
```