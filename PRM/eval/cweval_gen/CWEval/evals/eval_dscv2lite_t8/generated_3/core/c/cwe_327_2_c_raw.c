```c
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>

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
    // Use AES-256-CBC as it requires a 32-byte key
    if (key_len < 32) {
        fprintf(stderr, "Key length must be at least 32 bytes\n");
        exit(EXIT_FAILURE);
    }

    encryption_result result = {NULL, 0, NULL, NULL};
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char temp_key[32];

    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        fprintf(stderr, "Failed to generate IV\n");
        goto cleanup;
    }

    // Copy the first 32 bytes of the key for AES-256
    memcpy(temp_key, key, sizeof(temp_key));

    // Allocate memory for the encrypted data
    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        fprintf(stderr, "Memory allocation failed\n");
        goto cleanup;
    }

    // Encrypt the data
    AES_KEY aes_key;
    if (AES_set_encrypt_key(temp_key, 256, &aes_key) != 0) {
        fprintf(stderr, "Failed to set AES encryption key\n");
        free(result.encrypted_data);
        goto cleanup;
    }

    int padding = AES_BLOCK_SIZE - (data_len % AES_BLOCK_SIZE);
    if (padding == 0) padding = AES_BLOCK_SIZE;
    unsigned char* padded_data = malloc(data_len + padding);
    if (!padded_data) {
        fprintf(stderr, "Memory allocation failed\n");
        free(result.encrypted_data);
        goto cleanup;
    }
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, padding, padding);

    AES_cbc_encrypt(padded_data, result.encrypted_data, data_len + padding, &aes_key, iv, AES_ENCRYPT);

    // Store the lengths
    result.encrypted_len = data_len + padding;
    result.iv = iv;
    result.key = temp_key;

cleanup:
    free(padded_data);
    return result;
}
```