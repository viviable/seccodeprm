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
        fprintf(stderr, "Key length must be at least 32 bytes\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the result structure
    encryption_result result;
    result.key = malloc(32);
    if (!result.key) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(result.key, key, 32);

    // Generate a random IV
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.key);
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        free(result.key);
        free(result.iv);
        fprintf(stderr, "Failed to generate random IV\n");
        exit(EXIT_FAILURE);
    }

    // Calculate the padded data length
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = malloc(padded_len);
    if (!result.encrypted_data) {
        free(result.key);
        free(result.iv);
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Pad the data
    unsigned char* padded_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!padded_data) {
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, 0x00, AES_BLOCK_SIZE - data_len);

    // Encrypt the data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key, 256, &enc_key) != 0) {
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        free(padded_data);
        fprintf(stderr, "Failed to set encryption key\n");
        exit(EXIT_FAILURE);
    }
    AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, &enc_key, result.iv, AES_ENCRYPT);

    // Free temporary memory
    free(padded_data);

    // Set the encrypted data length
    result.encrypted_len = padded_len;

    return result;
}
```