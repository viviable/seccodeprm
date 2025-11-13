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
    if (key_len < AES_BLOCK_SIZE || data_len == 0) {
        return result; // Invalid input
    }

    // Allocate memory for IV and encrypted data
    unsigned char iv[AES_BLOCK_SIZE];
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        return result; // Memory allocation failed
    }
    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        free(result.iv);
        return result; // Memory allocation failed
    }

    // Generate random IV
    if (RAND_bytes(result.iv, AES_BLOCK_SIZE) != 1) {
        free(result.iv);
        free(result.encrypted_data);
        return result; // Failed to generate IV
    }

    // Use the first 32 bytes of the key as AES-256 key
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        free(result.iv);
        free(result.encrypted_data);
        return result; // Failed to set AES key
    }

    // Encrypt data
    AES_cbc_encrypt(data, result.encrypted_data + AES_BLOCK_SIZE, data_len, &aes_key, result.iv, AES_ENCRYPT);

    // Copy encrypted length including IV
    result.encrypted_len = data_len + AES_BLOCK_SIZE;

    // Copy key
    result.key = malloc(32); // AES-256 key length
    if (!result.key) {
        free(result.iv);
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, 32);

    return result;
}
```