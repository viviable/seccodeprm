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
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];

    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto end;
    }

    // Use the first 32 bytes of the key for AES-256-CBC
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        goto end;
    }

    // Calculate the length of the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        goto end;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, iv, AES_ENCRYPT);

    // Copy the IV and key
    result.iv = (unsigned char*)malloc(sizeof(iv));
    if (!result.iv) {
        free(result.encrypted_data);
        goto end;
    }
    memcpy(result.iv, iv, sizeof(iv));

    result.key = (unsigned char*)malloc(32);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        goto end;
    }
    memcpy(result.key, key, 32);

end:
    return result;
}
```