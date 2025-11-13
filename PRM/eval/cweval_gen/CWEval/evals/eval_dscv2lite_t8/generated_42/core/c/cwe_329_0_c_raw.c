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
    result.key = malloc(AES_BLOCK_SIZE);
    memcpy(result.key, key, AES_BLOCK_SIZE);

    result.iv = malloc(AES_BLOCK_SIZE);
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        // Handle error
        free(result.key);
        free(result.iv);
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        return result;
    }

    int num_blocks = (data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;
    result.encrypted_data = malloc(num_blocks * AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        // Handle error
        free(result.key);
        free(result.iv);
        result.encrypted_len = 0;
        return result;
    }
    result.encrypted_len = num_blocks * AES_BLOCK_SIZE;

    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, key_len * 8, &aes_key) != 0) {
        // Handle error
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        result.encrypted_len = 0;
        return result;
    }

    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, result.iv, AES_ENCRYPT);

    return result;
}
```