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
    if (key_len < AES_BLOCK_SIZE || data_len == 0) {
        return result; // Invalid input
    }

    // Generate random IV
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        return result; // Memory allocation failed
    }
    if (RAND_bytes(result.iv, AES_BLOCK_SIZE) != 1) {
        free(result.iv);
        return result; // Failed to generate IV
    }

    // Use the first 32 bytes of the key for AES encryption
    result.key = malloc(AES_BLOCK_SIZE);
    if (!result.key) {
        free(result.iv);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, AES_BLOCK_SIZE);

    // Allocate memory for encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE - data_len % AES_BLOCK_SIZE;
    result.encrypted_data = malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        free(result.iv);
        free(result.key);
        return result; // Memory allocation failed
    }

    AES_KEY aes_key;
    if (AES_set_encrypt_key(result.key, 128, &aes_key) != 0) {
        free(result.iv);
        free(result.key);
        free(result.encrypted_data);
        return result; // Failed to set up AES key
    }

    // Encrypt data
    int num_blocks = result.encrypted_len / AES_BLOCK_SIZE;
    for (int i = 0; i < num_blocks; ++i) {
        AES_cbc_encrypt(data + i * AES_BLOCK_SIZE, result.encrypted_data + i * AES_BLOCK_SIZE, AES_BLOCK_SIZE, &aes_key, result.iv, AES_ENCRYPT);
    }

    return result;
}
```