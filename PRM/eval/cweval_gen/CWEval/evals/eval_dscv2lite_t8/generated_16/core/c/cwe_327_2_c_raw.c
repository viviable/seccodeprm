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

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto end;
    }

    // Use AES-256-CBC for encryption
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        goto end;
    }

    // Calculate the total length of the encrypted data
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(padded_len);
    if (!result.encrypted_data) {
        goto end;
    }
    result.encrypted_len = padded_len;

    // Allocate memory for the IV
    result.iv = (unsigned char*)malloc(sizeof(iv));
    if (!result.iv) {
        free(result.encrypted_data);
        goto end;
    }
    memcpy(result.iv, iv, sizeof(iv));

    // Encrypt the data
    if (AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, iv, AES_ENCRYPT) != 0) {
        free(result.encrypted_data);
        free(result.iv);
        goto end;
    }

    // Copy the key
    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        goto end;
    }
    memcpy(result.key, key, key_len);

end:
    return result;
}
```