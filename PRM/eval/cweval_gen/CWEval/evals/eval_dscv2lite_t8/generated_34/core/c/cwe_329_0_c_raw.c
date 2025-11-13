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
    // Choose the appropriate key length based on the OpenSSL AES implementation
    AES_KEY enc_key;
    int aes_key_length = 0;
    if (key_len >= 32) {
        aes_key_length = AES_256_KEY_SIZE;
    } else if (key_len >= 16) {
        aes_key_length = AES_128_KEY_SIZE;
    } else if (key_len >= 8) {
        aes_key_length = AES_192_KEY_SIZE;
    }

    // Initialize the IV and encrypted data buffer
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char* encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted_data) {
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }
    memset(iv, 0, AES_BLOCK_SIZE);

    // Set up the AES key
    if (AES_set_encrypt_key(key, aes_key_length * 8, &enc_key) < 0) {
        free(encrypted_data);
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Prepare the result structure
    encryption_result result = {
        .encrypted_data = encrypted_data,
        .encrypted_len = data_len,
        .iv = iv,
        .key = (unsigned char*)malloc(aes_key_length);
    };
    memcpy(result.key, key, aes_key_length);

    return result;
}
```