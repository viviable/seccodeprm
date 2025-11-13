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
    // Determine the key length for AES (128, 192, or 256 bits)
    int aes_key_bits = key_len * 8;
    if (aes_key_bits != 128 && aes_key_bits != 192 && aes_key_bits != 256) {
        // Handle invalid key length
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Generate random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        // Handle error generating IV
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Allocate memory for the encrypted data
    size_t encrypted_len = data_len + AES_BLOCK_SIZE;
    unsigned char* encrypted_data = malloc(encrypted_len);
    if (!encrypted_data) {
        // Handle memory allocation failure
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Set up AES context
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, aes_key_bits, &enc_key) < 0) {
        // Handle error setting encryption key
        free(encrypted_data);
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Prepare the result structure
    encryption_result result = {
        .encrypted_data = encrypted_data,
        .encrypted_len = encrypted_len,
        .iv = iv,
        .key = (unsigned char*)malloc(key_len);
    };
    memcpy(result.key, key, key_len);

    return result;
}
```