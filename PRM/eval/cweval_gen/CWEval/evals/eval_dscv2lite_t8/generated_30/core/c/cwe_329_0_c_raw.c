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

    // Choose a key length based on OpenSSL's requirements
    size_t aes_key_len = AES_BLOCK_SIZE; // For simplicity, using AES-128

    // Allocate memory for the key
    if (key_len >= aes_key_len) {
        result.key = (unsigned char*)malloc(aes_key_len);
        memcpy(result.key, key, aes_key_len);
    } else {
        return result;
    }

    // Generate a random IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.key);
        return result;
    }
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        free(result.key);
        free(result.iv);
        return result;
    }

    // Allocate memory for the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        free(result.key);
        free(result.iv);
        return result;
    }

    // Encrypt the data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key, aes_key_len * 8, &enc_key) != 0) {
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        return result;
    }

    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, result.iv, AES_ENCRYPT);

    return result;
}
```