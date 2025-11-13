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
    unsigned char* encrypted_data = NULL;

    // Generate random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        goto cleanup;
    }

    // Use the first 32 bytes of the key for AES-128
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        goto cleanup;
    }

    // Calculate the encrypted length
    size_t encrypted_len = data_len + AES_BLOCK_SIZE - (data_len % AES_BLOCK_SIZE);
    encrypted_data = (unsigned char*)malloc(encrypted_len);
    if (!encrypted_data) {
        goto cleanup;
    }

    // Perform encryption
    int blocks = encrypted_len / AES_BLOCK_SIZE;
    AES_cbc_encrypt(data, encrypted_data, blocks * AES_BLOCK_SIZE, &aes_key, iv, AES_ENCRYPT);

    // Assign results
    result.encrypted_data = encrypted_data;
    result.encrypted_len = encrypted_len;
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    memcpy(result.iv, iv, AES_BLOCK_SIZE);
    result.key = (unsigned char*)malloc(16); // AES-128 key length
    memcpy(result.key, key, 16);

cleanup:
    if (!result.encrypted_data && encrypted_data) {
        free(encrypted_data);
    }
    return result;
}
```