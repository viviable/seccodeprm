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
    // Using AES-256-CBC as an example
    if (key_len < 32) {
        fprintf(stderr, "Key length must be at least 32 bytes for AES-256\n");
        exit(EXIT_FAILURE);
    }

    encryption_result result = {NULL, 0, NULL, NULL};
    result.key = malloc(32);
    memcpy(result.key, key, 32);

    // Generate random IV
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        fprintf(stderr, "Failed to generate random IV\n");
        free(result.key);
        return result;
    }

    // Allocate memory for encrypted data
    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        fprintf(stderr, "Failed to allocate memory for encrypted data\n");
        free(result.key);
        free(result.iv);
        return result;
    }
    result.encrypted_len = data_len + AES_BLOCK_SIZE;

    // Encrypt data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key, 256, &enc_key) != 0) {
        fprintf(stderr, "Failed to set AES encryption key\n");
        free(result.encrypted_data);
        free(result.key);
        free(result.iv);
        return result;
    }

    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, result.iv, AES_ENCRYPT);

    return result;
}
```