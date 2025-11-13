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
    // Ensure the key length is valid for AES-256
    if (key_len < 32) {
        // Handle error: key too short
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Create an instance of the encryption result structure
    encryption_result result;

    // Generate a random IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        // Handle memory allocation failure
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.key = NULL;
        return result;
    }
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        // Handle error generating IV
        free(result.iv);
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.key = NULL;
        return result;
    }

    // Set up the AES key
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, 256, &enc_key) < 0) {
        // Handle error setting AES key
        free(result.iv);
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.key = NULL;
        return result;
    }

    // Calculate the total encrypted length
    size_t total_encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(total_encrypted_len);
    if (!result.encrypted_data) {
        // Handle memory allocation failure
        free(result.iv);
        result.encrypted_len = 0;
        result.key = NULL;
        return result;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, result.iv, AES_ENCRYPT);

    // Duplicate the key for returning
    result.key = (unsigned char*)malloc(32);
    if (!result.key) {
        // Handle memory allocation failure
        free(result.iv);
        free(result.encrypted_data);
        result.encrypted_len = 0;
        return result;
    }
    memcpy(result.key, key, 32);

    // Store the length of the encrypted data
    result.encrypted_len = total_encrypted_len;

    return result;
}
```