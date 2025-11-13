```c
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <stdlib.h>

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
    int ret;

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto end;
    }

    // Use the first 32 bytes of the key as the AES key
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        goto end;
    }

    // Allocate memory for encrypted data
    encrypted = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted) {
        goto end;
    }

    // Encrypt data using AES-CBC mode
    ret = AES_cbc_encrypt(data, encrypted, data_len, &aes_key, iv, AES_ENCRYPT);
    if (ret != 1) {
        free(encrypted);
        encrypted = NULL;
        goto end;
    }

    // Set the result
    result.encrypted_data = encrypted;
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.iv = (unsigned char*)malloc(sizeof(iv));
    if (!result.iv) {
        free(encrypted);
        encrypted = NULL;
        goto end;
    }
    memcpy(result.iv, iv, sizeof(iv));
    result.key = (unsigned char*)malloc(16); // AES-128 key length
    if (!result.key) {
        free(result.iv);
        free(encrypted);
        encrypted = NULL;
        goto end;
    }
    memcpy(result.key, key, 16);

end:
    return result;
}
```