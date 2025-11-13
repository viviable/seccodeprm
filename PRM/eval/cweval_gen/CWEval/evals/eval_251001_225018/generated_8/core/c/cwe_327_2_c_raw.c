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
    unsigned char* iv = malloc(AES_BLOCK_SIZE);
    if (!iv) return result; // Memory allocation failed

    if (RAND_bytes(iv, AES_BLOCK_SIZE) != 1) {
        free(iv);
        return result; // Failed to generate IV
    }

    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
        free(iv);
        return result; // Failed to set encryption key
    }

    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = malloc(padded_len);
    if (!result.encrypted_data) {
        free(iv);
        return result; // Memory allocation failed
    }
    memset(result.encrypted_data, 0, padded_len);

    memcpy(result.encrypted_data, data, data_len);
    for (size_t i = data_len; i < padded_len; ++i) {
        result.encrypted_data[i] = padded_len - data_len;
    }

    AES_cbc_encrypt(result.encrypted_data, result.encrypted_data, padded_len, &enc_key, iv, AES_ENCRYPT);

    result.iv = iv;
    result.key = (unsigned char*)malloc(32); // Assuming 256-bit key
    if (!result.key) {
        free(result.encrypted_data);
        free(iv);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, 32);

    result.encrypted_len = padded_len;

    return result;
}
```