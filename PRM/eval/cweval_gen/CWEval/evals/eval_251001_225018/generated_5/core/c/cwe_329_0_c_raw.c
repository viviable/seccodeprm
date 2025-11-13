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
    // Ensure the key length is at least 32 bytes for AES-256
    if (key_len < 32) {
        // Handle error: key too short
        exit(EXIT_FAILURE);
    }

    // Initialize the result structure
    encryption_result result;
    result.key = (unsigned char*)malloc(32);
    memcpy(result.key, key, 32);

    // Generate a random IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        // Handle error: failed to generate IV
        free(result.key);
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);

    // Set up the AES context
    AES_KEY aes_key;
    if (AES_set_encrypt_key(result.key, 256, &aes_key) != 0) {
        // Handle error: failed to set encryption key
        free(result.iv);
        free(result.key);
        free(result.encrypted_data);
        exit(EXIT_FAILURE);
    }

    // Encrypt the data
    int blocks = data_len / AES_BLOCK_SIZE;
    int remaining = data_len % AES_BLOCK_SIZE;
    unsigned char buffer[AES_BLOCK_SIZE];
    memcpy(buffer, data, data_len);
    if (remaining > 0) {
        memset(buffer + remaining, 0, AES_BLOCK_SIZE - remaining);
    }

    for (int i = 0; i <= blocks; ++i) {
        AES_cbc_encrypt(buffer, result.encrypted_data + i * AES_BLOCK_SIZE, AES_BLOCK_SIZE, &aes_key, result.iv, AES_ENCRYPT);
        if (i < blocks) {
            memcpy(result.iv, result.encrypted_data + i * AES_BLOCK_SIZE, AES_BLOCK_SIZE);
        }
    }

    return result;
}
```