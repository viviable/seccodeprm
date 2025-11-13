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
    // Ensure the key length is at least 16 bytes for AES-128
    if (key_len < 16) {
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the result structure
    encryption_result result;
    result.encrypted_data = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    result.key = (unsigned char*)malloc(16);

    // Copy the key, ensuring it's exactly 16 bytes for AES-128
    memcpy(result.key, key, 16);

    // Generate a random IV
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        exit(EXIT_FAILURE);
    }

    // Create an AES context
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, 128, &enc_key) != 0) {
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        exit(EXIT_FAILURE);
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, result.iv, AES_ENCRYPT);

    // Set the length of the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;

    return result;
}
```