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
    if (data_len == 0 || key_len < 64) {
        return result; // Invalid input
    }

    // Use AES-256-CBC as the encryption algorithm
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char* encrypted = malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted) {
        return result; // Memory allocation failed
    }

    // Generate a random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        free(encrypted);
        return result; // Failed to generate IV
    }

    // Set up the AES key
    if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
        free(encrypted);
        return result; // Failed to set encryption key
    }

    // Pad the data to be a multiple of the block size
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* padded_data = malloc(padded_len);
    if (!padded_data) {
        free(encrypted);
        return result; // Memory allocation failed
    }
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, 0, padded_len - data_len);

    // Encrypt the data
    AES_cbc_encrypt(padded_data, encrypted, padded_len, &enc_key, iv, AES_ENCRYPT);

    // Free temporary buffers
    free(padded_data);

    // Store the results
    result.encrypted_data = encrypted;
    result.encrypted_len = padded_len;
    result.iv = iv;
    result.key = (unsigned char*)malloc(32); // 256-bit key
    if (result.key) {
        memcpy(result.key, key, 32);
    } else {
        // Clean up on error
        free(encrypted);
        free(iv);
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.iv = NULL;
    }

    return result;
}
```