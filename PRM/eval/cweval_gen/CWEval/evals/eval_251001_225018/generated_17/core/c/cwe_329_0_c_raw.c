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
    // Ensure we have at least 32 bytes for AES-256
    if (key_len < 32) {
        fprintf(stderr, "Key length must be at least 32 bytes\n");
        exit(EXIT_FAILURE);
    }

    // Initialize variables
    encryption_result result = {NULL, 0, NULL, NULL};
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char aes_key[32];

    // Copy the key to aes_key, ensuring it's exactly 32 bytes
    memset(aes_key, 0, sizeof(aes_key));
    memcpy(aes_key, key, 32);

    // Generate random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        fprintf(stderr, "Failed to generate IV\n");
        goto cleanup;
    }

    // Allocate memory for the encrypted data
    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        fprintf(stderr, "Memory allocation failed\n");
        goto cleanup;
    }

    // Set up AES context
    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 256, &enc_key) < 0) {
        fprintf(stderr, "Failed to set encryption key\n");
        free(result.encrypted_data);
        goto cleanup;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Store the length of the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;

    // Duplicate the IV and key for return
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        fprintf(stderr, "Memory allocation failed for IV\n");
        free(result.encrypted_data);
        goto cleanup;
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

    result.key = malloc(32);
    if (!result.key) {
        fprintf(stderr, "Memory allocation failed for key\n");
        free(result.encrypted_data);
        free(result.iv);
        goto cleanup;
    }
    memcpy(result.key, aes_key, 32);

cleanup:
    return result;
}
```