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
    // Ensure the key length is at least 32 bytes for AES-256
    if (key_len < 32) {
        return (encryption_result){NULL, 0, NULL, NULL};
    }

    // Allocate memory for the result structure
    encryption_result result;
    result.key = (unsigned char*)malloc(32);
    if (!result.key) {
        return (encryption_result){NULL, 0, NULL, NULL};
    }
    memcpy(result.key, key, 32);

    // Generate a random IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.key);
        return (encryption_result){NULL, 0, NULL, NULL};
    }
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        free(result.key);
        free(result.iv);
        return (encryption_result){NULL, 0, NULL, NULL};
    }

    // Encrypt the data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key, 256, &enc_key) != 0) {
        free(result.key);
        free(result.iv);
        return (encryption_result){NULL, 0, NULL, NULL};
    }

    // Calculate the padded length of the data
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(padded_len);
    if (!result.encrypted_data) {
        free(result.key);
        free(result.iv);
        return (encryption_result){NULL, 0, NULL, NULL};
    }

    // Perform the encryption
    AES_cbc_encrypt(data, result.encrypted_data, padded_len, &enc_key, result.iv, AES_ENCRYPT);

    // Store the actual encrypted length
    result.encrypted_len = padded_len;

    return result;
}
```