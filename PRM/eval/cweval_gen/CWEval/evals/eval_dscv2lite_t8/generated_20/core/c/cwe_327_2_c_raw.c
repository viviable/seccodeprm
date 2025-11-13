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

    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto cleanup;
    }

    // Use the first 32 bytes of the key as the AES key
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        goto cleanup;
    }

    // Allocate memory for the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        goto cleanup;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, iv, AES_ENCRYPT);

    // Copy the key and IV
    result.key = malloc(32);
    memcpy(result.key, key, 32);
    result.iv = malloc(AES_BLOCK_SIZE);
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

cleanup:
    if (result.encrypted_data) {
        free(result.encrypted_data);
    }
    if (result.key) {
        free(result.key);
    }
    if (result.iv) {
        free(result.iv);
    }

    return result;
}
```