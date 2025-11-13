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

    // Allocate memory for the key, IV, and encrypted data
    unsigned char* local_key = (unsigned char*)malloc(32);
    memcpy(local_key, key, 32);

    unsigned char* iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!iv) {
        // Handle error: failed to allocate IV
        free(local_key);
        encryption_result result = {NULL, 0, local_key, NULL};
        return result;
    }

    // Generate a random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        // Handle error: failed to generate random IV
        free(local_key);
        free(iv);
        encryption_result result = {NULL, 0, local_key, NULL};
        return result;
    }

    // Calculate the padded length of the data
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* padded_data = (unsigned char*)malloc(padded_len);
    if (!padded_data) {
        // Handle error: failed to allocate padded data
        free(local_key);
        free(iv);
        encryption_result result = {NULL, 0, local_key, NULL};
        return result;
    }
    memcpy(padded_data, data, data_len);
    // Pad the data with PKCS#7 padding
    memset(padded_data + data_len, data_len % AES_BLOCK_SIZE, data_len % AES_BLOCK_SIZE);

    // Encrypt the data
    unsigned char* encrypted_data = (unsigned char*)malloc(padded_len);
    if (!encrypted_data) {
        // Handle error: failed to allocate encrypted data
        free(local_key);
        free(iv);
        free(padded_data);
        encryption_result result = {NULL, 0, local_key, NULL};
        return result;
    }

    AES_KEY aes_key;
    if (AES_set_encrypt_key(local_key, 256, &aes_key) != 0) {
        // Handle error: failed to set AES encryption key
        free(local_key);
        free(iv);
        free(padded_data);
        free(encrypted_data);
        encryption_result result = {NULL, 0, local_key, NULL};
        return result;
    }

    AES_cbc_encrypt(padded_data, encrypted_data, padded_len, &aes_key, iv, AES_ENCRYPT);

    // Free allocated memory for intermediate data
    free(local_key);
    free(padded_data);

    // Prepare the result structure
    encryption_result result = {
        .encrypted_data = encrypted_data,
        .encrypted_len = padded_len,
        .iv = iv,
        .key = NULL  // Key is not returned; it's just used internally
    };

    return result;
}
```