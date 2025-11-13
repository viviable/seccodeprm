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
    // Ensure the key length is at least 32 bytes (AES-256)
    if (key_len < 32) {
        // Handle error: key too short
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Allocate memory for the encrypted data
    unsigned char* encrypted_data = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted_data) {
        // Handle error: memory allocation failed
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Allocate memory for the IV
    unsigned char* iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!iv) {
        // Free previously allocated memory
        free(encrypted_data);
        // Handle error: memory allocation failed
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Generate a random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        // Free previously allocated memory
        free(encrypted_data);
        free(iv);
        // Handle error: generating IV failed
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Set up the AES key schedule
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
        // Free previously allocated memory
        free(encrypted_data);
        free(iv);
        // Handle error: setting AES key failed
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Store the results
    encryption_result result = {
        .encrypted_data = encrypted_data,
        .encrypted_len = data_len,
        .iv = iv,
        .key = (unsigned char*)strndup((const char*)key, 32)
    };

    return result;
}
```