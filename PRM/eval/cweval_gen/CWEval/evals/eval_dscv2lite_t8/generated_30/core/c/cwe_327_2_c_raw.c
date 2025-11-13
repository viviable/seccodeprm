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
    if (data_len == 0 || key_len < AES_BLOCK_SIZE) {
        return result; // Invalid input
    }

    unsigned char aes_key[AES_BLOCK_SIZE];
    memcpy(aes_key, key, AES_BLOCK_SIZE);

    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        return result; // Failed to generate IV
    }

    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 128, &enc_key) != 0) {
        return result; // Failed to set up encryption key
    }

    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(padded_len);
    if (!result.encrypted_data) {
        return result; // Memory allocation failed
    }
    memset(result.encrypted_data, 0, padded_len);

    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    result.encrypted_len = padded_len;
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

    result.key = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Memory allocation failed
    }
    memcpy(result.key, aes_key, AES_BLOCK_SIZE);

    return result;
}
```