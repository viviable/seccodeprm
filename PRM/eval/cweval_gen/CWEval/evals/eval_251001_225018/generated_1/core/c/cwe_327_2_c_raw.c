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
    unsigned char iv[AES_BLOCK_SIZE];
    AES_KEY aes_key;

    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto cleanup;
    }

    // Use the first 32 bytes of the key for AES-256
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        goto cleanup;
    }

    // Allocate memory for the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        goto cleanup;
    }

    // Perform the encryption
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, iv, AES_ENCRYPT);

    // Store the key
    result.key = (unsigned char*)malloc(32);
    if (!result.key) {
        goto cleanup;
    }
    memcpy(result.key, key, 32);

    // Return the IV
    result.iv = iv;

    return result;

cleanup:
    if (result.encrypted_data) {
        free(result.encrypted_data);
    }
    if (result.key) {
        free(result.key);
    }
    memset(&iv, 0, sizeof(iv));
    return result;
}
```