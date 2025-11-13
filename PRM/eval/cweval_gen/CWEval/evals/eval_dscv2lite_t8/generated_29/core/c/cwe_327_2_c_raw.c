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
    unsigned char* encrypted = NULL;

    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto cleanup;
    }

    // Initialize AES key from the first 32 bytes of the key
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        goto cleanup;
    }

    // Allocate memory for the encrypted data
    encrypted = malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted) {
        goto cleanup;
    }

    // Pad the data to make its length a multiple of the block size
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    memset(encrypted + data_len, 0, padded_len - data_len);

    // Encrypt the data
    AES_cbc_encrypt(data, encrypted, padded_len, &aes_key, iv, AES_ENCRYPT);

    // Store the results
    result.encrypted_data = encrypted;
    result.encrypted_len = padded_len;
    result.iv = iv;
    result.key = (unsigned char*)key; // Note: This keeps a reference to the original key

cleanup:
    if (encrypted) {
        free(encrypted);
    }
    return result;
}
```