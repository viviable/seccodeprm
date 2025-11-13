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
    if (key_len < 32 || data_len == 0) {
        return result; // Early return for invalid input
    }

    // Ensure key length is compatible with AES-256
    const size_t effective_key_len = 32;
    unsigned char aes_key[effective_key_len];
    memcpy(aes_key, key, effective_key_len);

    // Allocate memory for IV and ciphertext
    unsigned char iv[AES_BLOCK_SIZE];
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        return result;
    }
    memset(result.iv, 0, AES_BLOCK_SIZE); // Initialize IV

    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        free(result.iv);
        return result;
    }
    result.encrypted_len = data_len + AES_BLOCK_SIZE;

    // Encrypt data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 256, &enc_key) != 0) {
        free(result.encrypted_data);
        free(result.iv);
        return result;
    }

    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Duplicate key for return value
    result.key = malloc(effective_key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result;
    }
    memcpy(result.key, aes_key, effective_key_len);

    return result;
}
```