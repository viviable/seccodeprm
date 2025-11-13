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
    encryption_result result = {NULL, 0, NULL, NULL};
    if (key_len < 16 || data_len == 0) {
        return result; // Invalid input
    }

    // Choose the appropriate key length based on OpenSSL's requirements
    int key_bits = 128;
    if (key_len >= 32) {
        key_bits = 256;
    } else if (key_len >= 24) {
        key_bits = 192;
    }

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Failed to generate IV
    }

    // Create an AES context
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, key_bits, &enc_key) != 0) {
        return result; // Failed to set up encryption key
    }

    // Allocate memory for the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE - data_len % AES_BLOCK_SIZE;
    result.encrypted_data = malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        return result; // Memory allocation failed
    }

    // Pad the data to make it a multiple of the block size
    unsigned char padded_data[result.encrypted_len];
    memcpy(padded_data, data, data_len);
    for (size_t i = data_len; i < result.encrypted_len; ++i) {
        padded_data[i] = result.encrypted_len - data_len;
    }

    // Encrypt the data
    AES_cbc_encrypt(padded_data, result.encrypted_data, result.encrypted_len, &enc_key, iv, AES_ENCRYPT);

    // Store the key and IV
    result.key = malloc(16); // Fixed key length for simplicity
    if (!result.key) {
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, 16);

    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        free(result.key);
        return result; // Memory allocation failed
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

    return result;
}
```