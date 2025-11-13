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
    encryption_result result;
    result.encrypted_data = NULL;
    result.encrypted_len = 0;
    result.iv = NULL;
    result.key = NULL;

    // Choose the appropriate AES key size based on the key length
    int aes_key_bits = 256; // Default to 256-bit AES
    if (key_len >= 32) {
        aes_key_bits = 256;
    } else if (key_len >= 16) {
        aes_key_bits = 192;
    } else if (key_len >= 8) {
        aes_key_bits = 128;
    }

    // Allocate memory for the IV and key
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        goto cleanup;
    }
    memcpy(result.iv, key, AES_BLOCK_SIZE);

    // Allocate memory for the key
    result.key = malloc(aes_key_bits / 8);
    if (!result.key) {
        goto cleanup;
    }
    memcpy(result.key, key, aes_key_bits / 8);

    // Allocate memory for the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        goto cleanup;
    }

    // Set up the AES context
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key, aes_key_bits, &enc_key) < 0) {
        goto cleanup;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, result.iv, AES_ENCRYPT);

cleanup:
    if (result.iv) {
        free(result.iv);
    }
    if (result.key) {
        free(result.key);
    }
    if (result.encrypted_data && !result.encrypted_len) {
        free(result.encrypted_data);
    }

    return result;
}
```