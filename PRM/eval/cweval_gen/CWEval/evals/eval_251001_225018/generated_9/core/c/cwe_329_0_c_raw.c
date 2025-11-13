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
    result.encrypted_data = NULL;
    result.encrypted_len = 0;
    result.iv = NULL;
    result.key = NULL;

    // Determine the key length based on OpenSSL's requirements
    int aes_key_length = 0;
    if (key_len >= 64) {
        aes_key_length = AES_BLOCK_SIZE * 8; // 512 bits
    } else if (key_len >= 48) {
        aes_key_length = AES_BLOCK_SIZE * 6; // 384 bits
    } else if (key_len >= 32) {
        aes_key_length = AES_BLOCK_SIZE * 4; // 256 bits
    } else if (key_len >= 24) {
        aes_key_length = AES_BLOCK_SIZE * 3; // 192 bits
    } else if (key_len >= 16) {
        aes_key_length = AES_BLOCK_SIZE * 2; // 128 bits
    } else {
        goto cleanup;
    }

    // Allocate memory for the IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        goto cleanup;
    }
    // Generate a random IV
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        goto cleanup;
    }

    // Allocate memory for the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        goto cleanup;
    }

    // Allocate memory for the key
    result.key = (unsigned char*)malloc(aes_key_length / 8);
    if (!result.key) {
        goto cleanup;
    }
    memcpy(result.key, key, aes_key_length / 8);

    // Encrypt the data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key, aes_key_length, &enc_key) != 0) {
        goto cleanup;
    }
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, result.iv, AES_ENCRYPT);

cleanup:
    if (!result.encrypted_data && !result.iv && !result.key) {
        return result;
    }

    free(result.encrypted_data);
    free(result.iv);
    free(result.key);
    memset(&result, 0, sizeof(encryption_result));
    return result;
}
```