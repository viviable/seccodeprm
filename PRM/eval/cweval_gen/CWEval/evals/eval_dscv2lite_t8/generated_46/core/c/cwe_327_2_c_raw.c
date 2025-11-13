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
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char *encrypted = NULL;

    // Generate a random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        goto cleanup;
    }

    // Use the first 32 bytes of the key for AES-128
    if (key_len >= 32) {
        if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
            goto cleanup;
        }
    } else {
        goto cleanup; // Key is too short for AES-128
    }

    // Allocate memory for the encrypted data
    encrypted = malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted) {
        goto cleanup;
    }

    // Pad the data to be a multiple of the block size
    while (data_len % AES_BLOCK_SIZE != 0) {
        data[data_len++] = '\0';
    }

    // Encrypt the data
    AES_cbc_encrypt(data, encrypted, data_len, &aes_key, iv, AES_ENCRYPT);

    // Store the results
    result.encrypted_data = encrypted;
    result.encrypted_len = data_len;
    result.iv = iv;
    result.key = (unsigned char*)malloc(32);
    memcpy(result.key, key, 32);

cleanup:
    if (!result.encrypted_data && encrypted) {
        free(encrypted);
    }
    if (!result.key) {
        free((void*)iv); // Cast away const to match the type expected by free
    }

    return result;
}
```