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
    // Ensure the key length is valid for AES
    if (key_len < AES_BLOCK_SIZE || key_len > 32 * AES_BLOCK_SIZE) {
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Create an AES key structure
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, key_len * 8, &aes_key) != 0) {
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Allocate memory for the encrypted data
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* encrypted_data = (unsigned char*)malloc(padded_len);
    if (encrypted_data == NULL) {
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, encrypted_data, padded_len, &aes_key, iv, AES_ENCRYPT);

    // Return the results
    encryption_result result = {
        .encrypted_data = encrypted_data,
        .encrypted_len = padded_len,
        .iv = iv,
        .key = (unsigned char*)malloc(key_len);
    };
    if (result.key == NULL) {
        free(encrypted_data);
        memset(&result, 0, sizeof(result));
        return result;
    }
    memcpy(result.key, key, key_len);

    return result;
}
```