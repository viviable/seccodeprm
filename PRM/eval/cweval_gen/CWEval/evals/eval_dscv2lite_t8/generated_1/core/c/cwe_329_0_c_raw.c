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

    // Generate random IV
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        return result; // Memory allocation failed
    }
    OpenSSL_add_all_algorithms();
    RAND_bytes(result.iv, AES_BLOCK_SIZE);

    // Select the appropriate key length for AES
    int aes_key_length = 16; // Default to 128-bit
    if (key_len >= 32) {
        aes_key_length = 32; // 256-bit
    } else if (key_len >= 24) {
        aes_key_length = 24; // 192-bit
    }

    // Create an AES key schedule
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, aes_key_length * 8, &enc_key) != 0) {
        free(result.iv);
        return result; // Key setup failed
    }

    // Calculate the encrypted length
    result.encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    // Allocate memory for encrypted data
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        free(result.iv);
        AES_cleanup();
        return result; // Memory allocation failed
    }

    // Perform AES encryption in CBC mode
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, result.iv, AES_ENCRYPT);

    // Store the key
    result.key = (unsigned char*)malloc(aes_key_length);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        AES_cleanup();
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, aes_key_length);

    return result;
}
```