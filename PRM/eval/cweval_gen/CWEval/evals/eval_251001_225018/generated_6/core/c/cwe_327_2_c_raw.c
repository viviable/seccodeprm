```c
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
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
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char temp_key[32]; // Using AES-256

    if (key_len < sizeof(temp_key)) {
        return result; // Key too short
    }

    memcpy(temp_key, key, sizeof(temp_key));

    // Initialize IV with random values
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Failed to get random bytes for IV
    }

    // Set up AES key
    if (AES_set_encrypt_key(temp_key, 256, &aes_key) < 0) {
        return result; // Failed to set AES key
    }

    // Allocate memory for encrypted data
    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        return result; // Memory allocation failed
    }
    result.encrypted_len = data_len;

    // Encrypt data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, iv, AES_ENCRYPT);

    // Duplicate key and IV for return
    result.key = malloc(sizeof(temp_key));
    if (!result.key) {
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.key, temp_key, sizeof(temp_key));

    result.iv = malloc(sizeof(iv));
    if (!result.iv) {
        free(result.encrypted_data);
        free(result.key);
        return result; // Memory allocation failed
    }
    memcpy(result.iv, iv, sizeof(iv));

    return result;
}
```