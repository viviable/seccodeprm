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
    if (key_len < AES_BLOCK_SIZE || data_len == 0) {
        return result;
    }

    // Use the first AES_BLOCK_SIZE bytes as the key
    unsigned char aes_key[AES_BLOCK_SIZE];
    memcpy(aes_key, key, AES_BLOCK_SIZE);

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result;
    }

    // Allocate memory for the encrypted data
    size_t encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(encrypted_len);
    if (!result.encrypted_data) {
        return result;
    }
    result.encrypted_len = encrypted_len;

    // Encrypt the data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 8 * AES_BLOCK_SIZE, &enc_key) != 0) {
        free(result.encrypted_data);
        return result;
    }

    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Store the IV and key
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        return result;
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

    result.key = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result;
    }
    memcpy(result.key, aes_key, AES_BLOCK_SIZE);

    return result;
}
```