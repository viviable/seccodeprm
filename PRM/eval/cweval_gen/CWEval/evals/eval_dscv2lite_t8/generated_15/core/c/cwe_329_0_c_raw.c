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
    unsigned char* encrypted = malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted) {
        return result; // Memory allocation failed
    }

    // Generate random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        free(encrypted);
        return result; // Failed to generate IV
    }

    // Set up AES key
    if (AES_set_encrypt_key(key, 8 * AES_BLOCK_SIZE, &enc_key) != 0) {
        free(encrypted);
        return result; // Failed to set AES key
    }

    // Encrypt data
    int len = 0;
    AES_cbc_encrypt(data, encrypted, data_len, &enc_key, iv, AES_ENCRYPT);

    // Store results
    result.encrypted_data = encrypted;
    result.encrypted_len = data_len;
    result.iv = iv;
    result.key = (unsigned char*)malloc(AES_BLOCK_SIZE);
    memcpy(result.key, key, AES_BLOCK_SIZE);

    return result;
}
```