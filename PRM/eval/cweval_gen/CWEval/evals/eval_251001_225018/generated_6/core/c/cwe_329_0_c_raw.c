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
    encryption_result result = {0};
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char* padded_data;
    size_t padded_len;

    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto error;
    }

    // Calculate padding length
    size_t pad_len = AES_BLOCK_SIZE - (data_len % AES_BLOCK_SIZE);
    padded_len = data_len + pad_len;
    padded_data = (unsigned char*)malloc(padded_len);
    if (!padded_data) {
        goto error;
    }

    // Copy data and add padding
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, pad_len, pad_len);

    // Set up AES key
    if (AES_set_encrypt_key(key, key_len * 8, &aes_key) != 0) {
        goto error;
    }

    // Allocate memory for encrypted data
    result.encrypted_data = (unsigned char*)malloc(padded_len);
    if (!result.encrypted_data) {
        goto error;
    }
    result.encrypted_len = padded_len;

    // Encrypt data
    AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, &aes_key, iv, AES_ENCRYPT);

    // Store key and IV
    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        goto error;
    }
    memcpy(result.key, key, key_len);
    result.iv = (unsigned char*)malloc(sizeof(iv));
    if (!result.iv) {
        goto error;
    }
    memcpy(result.iv, iv, sizeof(iv));

    free(padded_data);
    return result;

error:
    if (padded_data) free(padded_data);
    if (result.encrypted_data) free(result.encrypted_data);
    if (result.key) free(result.key);
    if (result.iv) free(result.iv);
    memset(&result, 0, sizeof(result));
    return result;
}
```