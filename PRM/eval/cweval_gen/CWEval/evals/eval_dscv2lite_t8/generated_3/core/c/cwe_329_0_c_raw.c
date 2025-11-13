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
    // Choose the appropriate AES key size based on the key length
    AES_KEY enc_key;
    int aes_key_size = 0;
    if (key_len >= 32) aes_key_size = AES_256_KEY_SIZE;
    else if (key_len >= 24) aes_key_size = AES_192_KEY_SIZE;
    else if (key_len >= 16) aes_key_size = AES_128_KEY_SIZE;
    else {
        // Handle error: key length too short
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        // Handle error: failed to generate random IV
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Allocate memory for the encrypted data
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* encrypted_data = (unsigned char*)malloc(padded_len);
    if (!encrypted_data) {
        // Handle error: failed to allocate memory
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Set up the AES key
    if (AES_set_encrypt_key(key, aes_key_size * 8, &enc_key) != 0) {
        // Handle error: failed to set AES key
        free(encrypted_data);
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Prepare the result structure
    encryption_result result = {
        .encrypted_data = encrypted_data,
        .encrypted_len = padded_len,
        .iv = iv,
        .key = (unsigned char*)malloc(aes_key_size);
    };
    memcpy(result.key, key, aes_key_size);

    return result;
}
```