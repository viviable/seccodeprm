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
        goto error;
    }

    // Use the first 32 bytes of the key for AES-128
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        goto error;
    }

    // Calculate the encrypted data length
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        goto error;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, iv, AES_ENCRYPT);

    // Duplicate the key for return
    result.key = malloc(key_len);
    if (!result.key) {
        goto error;
    }
    memcpy(result.key, key, key_len);

    // Duplicate the IV for return
    result.iv = malloc(sizeof(iv));
    if (!result.iv) {
        goto error;
    }
    memcpy(result.iv, iv, sizeof(iv));

    return result;

error:
    free(result.encrypted_data);
    free(result.key);
    free(result.iv);
    return result;
}
```