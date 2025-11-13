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
    // Determine the effective key length based on AES requirements
    size_t effective_key_len = key_len <= 32 ? key_len : (key_len <= 48 ? 32 : 48);
    unsigned char aes_key[AES_BLOCK_SIZE];
    memcpy(aes_key, key, effective_key_len);
    memset(aes_key + effective_key_len, 0, AES_BLOCK_SIZE - effective_key_len);

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        // Handle error: could not generate random bytes
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Allocate memory for the encrypted data
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* encrypted_data = malloc(padded_len);
    if (!encrypted_data) {
        // Handle error: could not allocate memory
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Encrypt the data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, effective_key_len * 8, &enc_key) != 0) {
        // Handle error: could not set encryption key
        free(encrypted_data);
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }
    AES_cbc_encrypt(data, encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Return the result
    encryption_result result = {
        .encrypted_data = encrypted_data,
        .encrypted_len = padded_len,
        .iv = iv,
        .key = aes_key
    };
    return result;
}
```