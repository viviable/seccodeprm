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
    encryption_result result;
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char* encrypted = NULL;
    int ret;

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto cleanup;
    }

    // Set up encryption key
    if ((ret = AES_set_encrypt_key(key, 256, &enc_key)) != 0) {
        AES_set_error(ret);
        goto cleanup;
    }

    // Allocate memory for encrypted data
    encrypted = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted) {
        goto cleanup;
    }

    // Encrypt data
    AES_cbc_encrypt(data, encrypted, data_len, &enc_key, iv, AES_ENCRYPT);

    // Store results
    result.encrypted_data = encrypted;
    result.encrypted_len = data_len;
    result.iv = iv;
    result.key = (unsigned char*)key; // Note: This assumes key is not modified

cleanup:
    if (!result.encrypted_data && encrypted) {
        free(encrypted);
    }
    return result;
}
```