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
        exit(EXIT_FAILURE);
    }

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        // Handle error: failed to generate IV
        exit(EXIT_FAILURE);
    }

    // Create an AES key schedule from the provided key
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        // Handle error: failed to set up AES key
        exit(EXIT_FAILURE);
    }

    // Calculate the padded data length
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* padded_data = (unsigned char*)malloc(padded_len);
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, 0, padded_len - data_len);

    // Allocate memory for the encrypted data
    unsigned char* encrypted_data = (unsigned char*)malloc(padded_len);
    if (!encrypted_data) {
        // Handle error: failed to allocate memory for encrypted data
        free(padded_data);
        exit(EXIT_FAILURE);
    }

    // Encrypt the data
    AES_cbc_encrypt(padded_data, encrypted_data, padded_len, &aes_key, iv, AES_ENCRYPT);

    // Free temporary memory
    free(padded_data);

    // Prepare the result structure
    encryption_result result = {
        .encrypted_data = encrypted_data,
        .encrypted_len = padded_len,
        .iv = iv,
        .key = (unsigned char*)malloc(32); // Only use the first 32 bytes of the key
        memcpy(result.key, key, 32);
    };

    return result;
}
```