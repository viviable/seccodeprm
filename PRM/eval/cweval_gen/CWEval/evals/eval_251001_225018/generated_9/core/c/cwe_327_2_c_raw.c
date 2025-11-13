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

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto cleanup;
    }

    // Initialize AES key
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        goto cleanup;
    }

    // Allocate memory for encrypted data
    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        goto cleanup;
    }
    result.encrypted_len = data_len + AES_BLOCK_SIZE;

    // Encrypt data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, iv, AES_ENCRYPT);

    // Copy key for return
    result.key = malloc(key_len);
    if (!result.key) {
        goto cleanup;
    }
    memcpy(result.key, key, key_len);

cleanup:
    // Free allocated memory in case of error
    if (!result.encrypted_data || !result.key) {
        free(result.encrypted_data);
        free(result.key);
        memset(&result, 0, sizeof(encryption_result));
    }

    return result;
}
```