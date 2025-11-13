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
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char* encrypted = NULL;
    int num_rounds = 12; // AES-192

    if (key_len < 32 || data_len == 0) {
        return result;
    }

    // Initialize the IV
    memcpy(iv, key + key_len - AES_BLOCK_SIZE, AES_BLOCK_SIZE);

    // Set up encryption key
    if (AES_set_encrypt_key(key, 192, &enc_key) != 0) {
        return result;
    }

    // Allocate memory for encrypted data
    encrypted = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted) {
        return result;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, encrypted, data_len, &enc_key, iv, AES_ENCRYPT);

    // Store results
    result.encrypted_data = encrypted;
    result.encrypted_len = data_len;
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (result.iv) {
        memcpy(result.iv, iv, AES_BLOCK_SIZE);
    }
    result.key = (unsigned char*)malloc(24); // AES-192 key size
    if (result.key) {
        memcpy(result.key, key + key_len - 24, 24);
    }

    return result;
}
```