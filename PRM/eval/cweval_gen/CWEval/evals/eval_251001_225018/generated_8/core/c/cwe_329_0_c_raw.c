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
    if (key_len < 16 || data_len == 0) {
        return result; // Invalid input
    }

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Failed to generate IV
    }

    // Determine the effective key length (must be multiple of 8)
    size_t key_effective_len = key_len % 8 == 0 ? key_len : key_len - (key_len % 8);

    // Allocate memory for the encrypted data
    result.encrypted_data = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        return result; // Memory allocation failed
    }
    result.encrypted_len = data_len + AES_BLOCK_SIZE;

    // Encrypt the data
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, key_effective_len * 8, &aes_key) != 0) {
        free(result.encrypted_data);
        return result; // Encryption key setup failed
    }

    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, iv, AES_ENCRYPT);

    // Store the key
    result.key = (unsigned char*)malloc(key_effective_len);
    if (!result.key) {
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, key_effective_len);

    // Store the IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        free(result.key);
        return result; // Memory allocation failed
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

    return result;
}
```