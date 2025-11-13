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
    encryption_result result = {NULL, 0, NULL, NULL};
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];

    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto end;
    }

    // Set up encryption key
    if (AES_set_encrypt_key(key, key_len * 8, &enc_key) != 0) {
        goto end;
    }

    // Allocate memory for encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        goto end;
    }

    // Encrypt data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Duplicate the key for return
    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        free(result.encrypted_data);
        goto end;
    }
    memcpy(result.key, key, key_len);

    // Duplicate the IV for return
    result.iv = (unsigned char*)malloc(sizeof(iv));
    if (!result.iv) {
        free(result.encrypted_data);
        free(result.key);
        goto end;
    }
    memcpy(result.iv, iv, sizeof(iv));

end:
    return result;
}
```