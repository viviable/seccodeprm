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
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char* padded_data;
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    size_t encrypted_len = padded_len;

    // Allocate memory for the padded data and the encrypted data
    padded_data = (unsigned char*)malloc(padded_len);
    if (!padded_data) {
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }

    // Copy the original data into the padded data and pad it
    memcpy(padded_data, data, data_len);
    while (data_len < padded_len) {
        padded_data[data_len++] = 0x01;
    }

    // Allocate memory for the IV and the encrypted data
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(padded_data);
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.key = NULL;
        return result;
    }

    // Generate a random IV
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        free(padded_data);
        free(result.iv);
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.key = NULL;
        return result;
    }

    // Set up the encryption key
    if (AES_set_encrypt_key(key, key_len * 8, &enc_key) < 0) {
        free(padded_data);
        free(result.iv);
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.key = NULL;
        return result;
    }

    // Encrypt the data
    result.encrypted_data = (unsigned char*)malloc(encrypted_len);
    if (!result.encrypted_data) {
        free(padded_data);
        free(result.iv);
        result.encrypted_len = 0;
        result.key = NULL;
        return result;
    }
    AES_cbc_encrypt(padded_data, result.encrypted_data, encrypted_len, &enc_key, result.iv, AES_ENCRYPT);

    // Free the padded data
    free(padded_data);

    // Store the key
    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        result.encrypted_len = 0;
        return result;
    }
    memcpy(result.key, key, key_len);

    // Return the result
    result.encrypted_len = encrypted_len;
    return result;
}
```