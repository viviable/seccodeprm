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

    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char* padded_data;
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    padded_data = (unsigned char*)malloc(padded_len);
    if (!padded_data) {
        return result; // Memory allocation failed
    }

    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, 0, padded_len - data_len); // Padding with zeros

    result.key = (unsigned char*)malloc(16);
    if (!result.key) {
        free(padded_data);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, 16);

    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(padded_data);
        free(result.key);
        return result; // Memory allocation failed
    }
    randombytes_buf(result.iv, AES_BLOCK_SIZE); // Generate a random IV

    if (AES_set_encrypt_key(key, 128, &enc_key) != 0) {
        free(padded_data);
        free(result.key);
        free(result.iv);
        return result; // Key setup failed
    }

    result.encrypted_data = (unsigned char*)malloc(padded_len);
    if (!result.encrypted_data) {
        free(padded_data);
        free(result.key);
        free(result.iv);
        return result; // Memory allocation failed
    }

    AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, &enc_key, result.iv, AES_ENCRYPT);

    free(padded_data);

    result.encrypted_len = padded_len;

    return result;
}
```