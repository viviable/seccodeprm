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
        exit(EXIT_FAILURE);
    }

    // Create a buffer for the encrypted data
    encryption_result result;
    result.encrypted_data = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        exit(EXIT_FAILURE);
    }

    // Initialize the IV with random bytes
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        exit(EXIT_FAILURE);
    }
    AES_random_key(result.iv);

    // Select the appropriate AES key length based on the key length
    AES_KEY enc_key;
    int aes_key_bits = 0;
    if (key_len >= 32) {
        aes_key_bits = 256;
    } else if (key_len >= 24) {
        aes_key_bits = 192;
    } else if (key_len >= 16) {
        aes_key_bits = 128;
    }

    // Set up the encryption key
    if (AES_set_encrypt_key(key, aes_key_bits, &enc_key) != 0) {
        free(result.encrypted_data);
        free(result.iv);
        exit(EXIT_FAILURE);
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, result.iv, AES_ENCRYPT);

    // Store the key used for encryption
    result.key = (unsigned char*)malloc(aes_key_bits / 8);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        exit(EXIT_FAILURE);
    }
    memcpy(result.key, key, aes_key_bits / 8);

    // Calculate the actual length of the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;

    return result;
}
```