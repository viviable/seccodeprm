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

    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char* padded_data = NULL;
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Failed to generate IV
    }

    // Pad data
    padded_data = (unsigned char*)malloc(padded_len);
    if (!padded_data) {
        return result; // Memory allocation failed
    }
    memcpy(padded_data, data, data_len);
    while (data_len < padded_len) {
        padded_data[data_len++] = 0; // PKCS#7 padding
    }

    // Encrypt data
    if (AES_set_encrypt_key(key, 8 * AES_BLOCK_SIZE, &aes_key) != 0) {
        free(padded_data);
        return result; // Failed to set AES key
    }

    result.encrypted_data = (unsigned char*)malloc(padded_len);
    if (!result.encrypted_data) {
        free(padded_data);
        return result; // Memory allocation failed
    }

    AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, &aes_key, iv, AES_ENCRYPT);

    // Free resources
    free(padded_data);

    result.encrypted_len = padded_len;
    result.iv = iv;
    result.key = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (result.key) {
        memcpy(result.key, key, AES_BLOCK_SIZE);
    } else {
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        free(result.iv);
        result.iv = NULL;
    }

    return result;
}
```