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
    // Use AES-256-CBC as the chosen algorithm
    if (key_len < 32) {
        fprintf(stderr, "Key length must be at least 32 bytes for AES-256\n");
        exit(EXIT_FAILURE);
    }

    encryption_result result = {NULL, 0, NULL, NULL};
    unsigned char* iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!iv) {
        fprintf(stderr, "Memory allocation failed\n");
        goto cleanup;
    }

    unsigned char* key_used = (unsigned char*)malloc(32);
    if (!key_used) {
        fprintf(stderr, "Memory allocation failed\n");
        goto cleanup;
    }
    memcpy(key_used, key, 32);

    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        fprintf(stderr, "Failed to generate random IV\n");
        goto cleanup;
    }

    AES_KEY aes_key;
    if (AES_set_encrypt_key(key_used, 256, &aes_key) != 0) {
        fprintf(stderr, "Failed to set AES encryption key\n");
        goto cleanup;
    }

    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(padded_len);
    if (!result.encrypted_data) {
        fprintf(stderr, "Memory allocation failed\n");
        goto cleanup;
    }

    result.encrypted_len = padded_len;
    int pad_len = padded_len - data_len;
    memset(result.encrypted_data + data_len, pad_len, pad_len);

    AES_cbc_encrypt(data, result.encrypted_data, padded_len, &aes_key, iv, AES_ENCRYPT);

    result.iv = iv;
    result.key = key_used;

cleanup:
    free(iv);
    free(key_used);
    return result;
}
```