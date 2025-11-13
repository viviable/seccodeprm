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
    // Ensure the key length is at least 128 bits (16 bytes)
    if (key_len < 16) {
        return (encryption_result){NULL, 0, NULL, NULL};
    }

    // Allocate memory for the encrypted data
    encryption_result result;
    result.encrypted_data = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        return result;
    }
    memset(result.encrypted_data, 0, data_len + AES_BLOCK_SIZE);

    // Allocate memory for the IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        return result;
    }
    memset(result.iv, 0, AES_BLOCK_SIZE);

    // Set up the AES key
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        free(result.encrypted_data);
        free(result.iv);
        return result;
    }

    // Encrypt the data
    int blocks = (data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, result.iv, AES_ENCRYPT);

    // Set the lengths
    result.encrypted_len = data_len;

    // Copy the key used for encryption
    result.key = (unsigned char*)malloc(16); // Always use 128-bit key
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result;
    }
    memcpy(result.key, key, 16);

    return result;
}
```