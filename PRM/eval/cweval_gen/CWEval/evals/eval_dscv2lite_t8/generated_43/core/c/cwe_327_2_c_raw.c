```c
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>

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
    if (key_len < 64 || data_len == 0) {
        return result; // Invalid input
    }

    // Using AES-256-CBC as the chosen algorithm
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char *encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted_data) {
        return result; // Memory allocation failed
    }
    memset(encrypted_data, 0, data_len + AES_BLOCK_SIZE);

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        free(encrypted_data);
        return result; // Failed to generate IV
    }

    // Set up encryption key
    if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
        free(encrypted_data);
        return result; // Failed to set encryption key
    }

    // Encrypt data
    AES_cbc_encrypt(data, encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    result.encrypted_data = encrypted_data;
    result.encrypted_len = data_len;
    result.iv = malloc(sizeof(iv));
    if (!result.iv) {
        free(encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.iv, iv, sizeof(iv));

    result.key = malloc(32); // AES-256 key length
    if (!result.key) {
        free(encrypted_data);
        free(result.iv);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, 32);

    return result;
}
```