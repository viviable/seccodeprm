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

    // Allocate memory for the result structure
    encryption_result result;
    result.key = malloc(32); // AES-256 key length
    memcpy(result.key, key, 32);

    // Generate a random IV
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        // Handle error: failed to generate random IV
        free(result.key);
        exit(EXIT_FAILURE);
    }

    // Encrypt the data
    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key, 256, &enc_key) != 0) {
        // Handle error: failed to set encryption key
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        exit(EXIT_FAILURE);
    }

    int padding_length = AES_BLOCK_SIZE - (data_len % AES_BLOCK_SIZE);
    unsigned char padded_data[data_len + padding_length];
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, padding_length, padding_length);

    AES_cbc_encrypt(padded_data, result.encrypted_data, data_len + padding_length, &enc_key, result.iv, AES_ENCRYPT);

    // Calculate the actual length of the encrypted data
    result.encrypted_len = data_len + padding_length;

    return result;
}
```