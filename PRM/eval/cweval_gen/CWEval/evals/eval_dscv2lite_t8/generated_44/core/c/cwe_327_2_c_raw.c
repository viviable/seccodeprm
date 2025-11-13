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
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        goto cleanup;
    }

    // Use the first 32 bytes of the key as AES-256 key
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        goto cleanup;
    }

    // Calculate the encrypted length
    size_t encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    // Allocate memory for the encrypted data
    encrypted = (unsigned char*)malloc(encrypted_len);
    if (!encrypted) {
        goto cleanup;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, encrypted, encrypted_len, &aes_key, iv, AES_ENCRYPT);

    // Assign the results
    result.encrypted_data = encrypted;
    result.encrypted_len = encrypted_len;
    result.iv = iv;
    result.key = (unsigned char*)key; // Note: This returns a pointer to the original key

cleanup:
    return result;
}
```