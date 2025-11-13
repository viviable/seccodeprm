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
    // Ensure the key length is at least 128 bits (16 bytes)
    if (key_len < 16) {
        exit(EXIT_FAILURE);
    }

    // Initialize variables
    encryption_result result;
    result.key = (unsigned char*)malloc(16);
    memcpy(result.key, key, 16);

    // Generate random IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        free(result.key);
        exit(EXIT_FAILURE);
    }

    // Allocate memory for encrypted data
    result.encrypted_data = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        free(result.key);
        free(result.iv);
        exit(EXIT_FAILURE);
    }
    result.encrypted_len = data_len + AES_BLOCK_SIZE;

    // Encrypt data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key, 128, &enc_key) != 0) {
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        exit(EXIT_FAILURE);
    }

    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, result.iv, AES_ENCRYPT);

    return result;
}
```