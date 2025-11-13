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
    // Determine the appropriate key length based on OpenSSL's requirements
    int aes_key_length = AES_BLOCK_SIZE; // AES-128
    if (key_len >= 32) {
        aes_key_length = AES_KEY_LENGTH_256; // AES-256
    } else if (key_len >= 24) {
        aes_key_length = AES_KEY_LENGTH_192; // AES-192
    }

    // Allocate memory for the result structure
    encryption_result result;
    result.key = malloc(aes_key_length);
    memcpy(result.key, key, aes_key_length);

    // Generate a random IV
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        free(result.key);
        result.key = NULL;
        result.iv = NULL;
        return result;
    }

    // Create an AES key schedule
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key, aes_key_length * 8, &enc_key) != 0) {
        free(result.key);
        free(result.iv);
        result.key = NULL;
        result.iv = NULL;
        return result;
    }

    // Calculate the total length of the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = malloc(result.encrypted_len);

    // Pad the input data to make it a multiple of the block size
    unsigned char padding[AES_BLOCK_SIZE];
    size_t padding_len = AES_BLOCK_SIZE - (data_len % AES_BLOCK_SIZE);
    memset(padding, padding_len, padding_len);

    // Encrypt the data using AES-CBC mode
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, result.iv, AES_ENCRYPT);

    // Copy the padding to the end of the encrypted data
    memcpy(result.encrypted_data + data_len, padding, padding_len);

    return result;
}
```