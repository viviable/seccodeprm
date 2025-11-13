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
        exit(EXIT_FAILURE);
    }

    // Create an encryption result structure
    encryption_result result = {NULL, 0, NULL, NULL};

    // Allocate memory for the IV and the encrypted data
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    result.key = (unsigned char*)malloc(32); // AES-256 key length

    // Copy the first 32 bytes of the key for AES-256
    memcpy(result.key, key, 32);

    // Initialize the IV with random bytes
    AES_random_key(result.iv, AES_BLOCK_SIZE);

    // Calculate the padded data length
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* padded_data = (unsigned char*)malloc(padded_len);
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, 0x00, padded_len - data_len);

    // Encrypt the data
    result.encrypted_data = (unsigned char*)malloc(padded_len);
    AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, (AES_KEY*)result.key, result.iv, AES_ENCRYPT);

    // Free the allocated memory for padded data
    free(padded_data);

    // Set the encrypted length
    result.encrypted_len = padded_len;

    return result;
}
```