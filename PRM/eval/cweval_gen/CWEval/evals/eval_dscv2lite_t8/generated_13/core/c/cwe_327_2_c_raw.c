```c
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>

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

    // Initialize AES key
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        goto cleanup;
    }

    // Allocate memory for encrypted data
    encrypted = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted) {
        goto cleanup;
    }

    // Encrypt data
    AES_cbc_encrypt(data, encrypted, data_len, &aes_key, iv, AES_ENCRYPT);

    // Assign results
    result.encrypted_data = encrypted;
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.iv = (unsigned char*)malloc(sizeof(iv));
    memcpy(result.iv, iv, sizeof(iv));
    result.key = (unsigned char*)malloc(32); // Assuming we use 256-bit AES
    memcpy(result.key, key, 32);

cleanup:
    if (!result.encrypted_data) {
        free(encrypted);
    }
    return result;
}
```