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
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];
    int num_blocks = (data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE;
    size_t encrypted_len = num_blocks * AES_BLOCK_SIZE;

    // Allocate memory for the encrypted data and IV
    result.encrypted_data = malloc(encrypted_len);
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.encrypted_data || !result.iv) {
        goto cleanup;
    }

    // Generate a random IV
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        goto cleanup;
    }

    // Set up the AES key
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        goto cleanup;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, encrypted_len, &aes_key, result.iv, AES_ENCRYPT);

    result.encrypted_len = encrypted_len;
    result.key = (unsigned char*)key; // Store the key pointer

cleanup:
    if (result.encrypted_data && !result.key) {
        free(result.encrypted_data);
    }
    if (result.iv) {
        free(result.iv);
    }

    return result;
}
```