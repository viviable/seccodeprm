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

    // Generate random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        return result; // Failed to generate IV
    }
    result.iv = malloc(AES_BLOCK_SIZE);
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

    // Copy key (prefix of key if too long)
    unsigned char* effective_key = (unsigned char*)malloc(key_len >= AES_KEY_LENGTH ? AES_KEY_LENGTH : key_len);
    memcpy(effective_key, key, effective_key);

    // Allocate memory for encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = malloc(result.encrypted_len);
    if (!result.encrypted_data || !effective_key) {
        free(result.iv);
        free(effective_key);
        return result; // Memory allocation failed
    }

    // Set up AES encryption context
    AES_KEY enc_key;
    if (AES_set_encrypt_key(effective_key, key_len * 8, &enc_key) != 0) {
        free(result.iv);
        free(result.encrypted_data);
        free(effective_key);
        return result; // Key setup failed
    }

    // Encrypt data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Free temporary key
    free(effective_key);

    return result;
}
```