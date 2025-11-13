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
    unsigned char aes_key[AES_BLOCK_SIZE];
    unsigned char des_key[DES_KEY_SZ];
    unsigned char iv[AES_BLOCK_SIZE];

    // Generate random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        goto error;
    }

    // Copy the appropriate part of the key to the AES key
    memcpy(aes_key, key, sizeof(aes_key));

    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 128, &enc_key) != 0) {
        goto error;
    }

    // Allocate memory for the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        goto error;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Store the key and IV
    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        goto error;
    }
    memcpy(result.key, key, key_len);

    return result;

error:
    if (result.encrypted_data) {
        free(result.encrypted_data);
    }
    if (result.iv) {
        free(result.iv);
    }
    if (result.key) {
        free(result.key);
    }
    memset(&result, 0, sizeof(encryption_result));
    return result;
}
```