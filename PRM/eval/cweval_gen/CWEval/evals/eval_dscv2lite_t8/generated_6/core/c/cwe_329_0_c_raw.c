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
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];

    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Return empty result on failure
    }

    // Set up encryption key
    if (key_len == 32) {
        if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
            return result; // Return empty result on failure
        }
    } else if (key_len == 24) {
        if (AES_set_encrypt_key(key, 192, &enc_key) != 0) {
            return result; // Return empty result on failure
        }
    } else if (key_len == 16) {
        if (AES_set_encrypt_key(key, 128, &enc_key) != 0) {
            return result; // Return empty result on failure
        }
    } else {
        return result; // Return empty result on invalid key length
    }

    // Calculate the length of the encrypted data
    size_t encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    // Allocate memory for the encrypted data
    result.encrypted_data = malloc(encrypted_len);
    if (!result.encrypted_data) {
        return result; // Return empty result on memory allocation failure
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Store the IV and key
    result.iv = malloc(sizeof(iv));
    if (!result.iv) {
        free(result.encrypted_data);
        return result; // Return empty result on memory allocation failure
    }
    memcpy(result.iv, iv, sizeof(iv));

    result.key = malloc(key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Return empty result on memory allocation failure
    }
    memcpy(result.key, key, key_len);

    // Set the length of the encrypted data
    result.encrypted_len = encrypted_len;

    return result;
}
```