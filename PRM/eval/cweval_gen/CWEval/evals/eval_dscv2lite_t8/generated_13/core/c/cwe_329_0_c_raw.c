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
    if (key_len < AES_BLOCK_SIZE || data_len == 0) {
        return result; // Invalid input
    }

    // Create a buffer for the encrypted data
    result.encrypted_data = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        return result; // Memory allocation failed
    }
    memset(result.encrypted_data, 0, data_len + AES_BLOCK_SIZE);

    // Create a buffer for the IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memset(result.iv, 0, AES_BLOCK_SIZE);

    // Copy the IV from the beginning of the key
    memcpy(result.iv, key, AES_BLOCK_SIZE);

    // Set up the AES key schedule
    AES_KEY aes_key;
    int ret = AES_set_encrypt_key(key, 256, &aes_key); // Assuming 256-bit key
    if (ret != 0) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Key setup failed
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, result.iv, AES_ENCRYPT);

    // Store the actual encrypted length
    result.encrypted_len = data_len;

    // Save the key
    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, key_len);

    return result;
}
```