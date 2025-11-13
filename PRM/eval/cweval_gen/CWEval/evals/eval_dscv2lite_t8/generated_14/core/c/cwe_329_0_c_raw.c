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
    unsigned char *encrypted = malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted) {
        return result; // Memory allocation failed
    }

    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        free(encrypted);
        return result; // Failed to generate IV
    }

    // Set up AES encryption key
    if (AES_set_encrypt_key(key, key_len * 8, &enc_key) != 0) {
        free(encrypted);
        return result; // Key setup failed
    }

    // Encrypt data
    AES_cbc_encrypt(data, encrypted, data_len, &enc_key, iv, AES_ENCRYPT);

    result.encrypted_data = encrypted;
    result.encrypted_len = data_len;
    result.iv = malloc(sizeof(iv));
    if (!result.iv) {
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.iv, iv, sizeof(iv));

    result.key = malloc(key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, key_len);

    return result;
}
```