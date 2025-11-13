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
    encryption_result result = {0};
    if (key_len < AES_BLOCK_SIZE || data_len == 0) {
        return result; // Invalid input
    }

    // Generate random IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        return result; // Memory allocation failed
    }
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        free(result.iv);
        return result; // Failed to generate IV
    }

    // Create key schedule
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
        free(result.iv);
        return result; // Failed to set encryption key
    }

    // Calculate encrypted length
    result.encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        free(result.iv);
        return result; // Memory allocation failed
    }

    // Encrypt data
    AES_cbc_encrypt(data, result.encrypted_data, result.encrypted_len, &enc_key, result.iv, AES_ENCRYPT);

    // Copy key for return
    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        free(result.iv);
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, key_len);

    return result;
}
```