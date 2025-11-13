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
    // Ensure the key length is at least 16 bytes (AES-128)
    if (key_len < 16) {
        fprintf(stderr, "Key must be at least 16 bytes\n");
        exit(EXIT_FAILURE);
    }

    // Create an instance to store the result
    encryption_result result = {NULL, 0, NULL, NULL};

    // Generate a random IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        perror("Failed to allocate memory for IV");
        goto cleanup;
    }
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        fprintf(stderr, "Failed to generate random IV\n");
        goto cleanup;
    }

    // Allocate memory for the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE; // Pad to full blocks
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        perror("Failed to allocate memory for encrypted data");
        goto cleanup;
    }

    // Set up the AES context
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, 128, &enc_key) < 0) {
        fprintf(stderr, "Failed to set AES encryption key\n");
        goto cleanup;
    }

    // Encrypt the data
    int num_blocks = (data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;
    for (int i = 0; i < num_blocks; ++i) {
        AES_cbc_encrypt(data + i * AES_BLOCK_SIZE, 
                        result.encrypted_data + i * AES_BLOCK_SIZE, 
                        AES_BLOCK_SIZE, 
                        &enc_key, 
                        result.iv, 
                        AES_ENCRYPT);
    }

    // Copy the key into the result structure
    result.key = (unsigned char*)malloc(16); // Always use 128-bit key for simplicity
    if (!result.key) {
        perror("Failed to allocate memory for key");
        goto cleanup;
    }
    memcpy(result.key, key, 16);

cleanup:
    // Free allocated memory in case of failure
    free(result.iv);
    free(result.encrypted_data);
    free(result.key);

    return result;
}
```