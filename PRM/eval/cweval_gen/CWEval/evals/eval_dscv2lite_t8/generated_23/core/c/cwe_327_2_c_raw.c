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
    // For simplicity, let's use AES-256-CBC
    if (key_len < AES_BLOCK_SIZE * 2) {
        fprintf(stderr, "Key too short for AES-256\n");
        exit(EXIT_FAILURE);
    }

    encryption_result result = {0};
    result.key = (unsigned char*)malloc(AES_BLOCK_SIZE * 2);
    memcpy(result.key, key, AES_BLOCK_SIZE * 2);

    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        fprintf(stderr, "Failed to generate random IV\n");
        free(result.key);
        exit(EXIT_FAILURE);
    }

    result.encrypted_data = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        fprintf(stderr, "Memory allocation failed\n");
        free(result.key);
        free(result.iv);
        exit(EXIT_FAILURE);
    }

    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        fprintf(stderr, "Failed to set AES key\n");
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        exit(EXIT_FAILURE);
    }

    int len = 0;
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, result.iv, AES_ENCRYPT);
    result.encrypted_len = data_len;

    return result;
}
```