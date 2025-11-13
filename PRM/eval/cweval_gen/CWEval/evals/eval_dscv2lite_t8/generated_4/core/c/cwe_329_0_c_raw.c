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

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        return result; // Failed to generate IV
    }

    // Create a buffer for the encrypted data
    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        return result; // Memory allocation failed
    }
    result.encrypted_len = data_len + AES_BLOCK_SIZE;

    // Set up the AES key
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        free(result.encrypted_data);
        return result; // Failed to set key
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data + AES_BLOCK_SIZE, data_len, &aes_key, iv, AES_ENCRYPT);

    // Copy the IV and key to the result structure
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

    result.key = malloc(32); // AES-256 key length
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, 32);

    return result;
}
```