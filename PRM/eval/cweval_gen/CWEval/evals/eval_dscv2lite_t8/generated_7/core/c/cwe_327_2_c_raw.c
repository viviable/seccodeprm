```c
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <stdlib.h>

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
    
    // Use AES-256-CBC as the chosen algorithm
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
        goto cleanup;
    }

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto cleanup;
    }
    
    // Calculate the length of the encrypted data
    size_t encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = malloc(encrypted_len);
    if (!result.encrypted_data) {
        goto cleanup;
    }
    result.encrypted_len = encrypted_len;
    
    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Copy the IV and key to the result structure
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        goto cleanup;
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

    result.key = malloc(key_len);
    if (!result.key) {
        goto cleanup;
    }
    memcpy(result.key, key, key_len);

    return result;

cleanup:
    free(result.encrypted_data);
    free(result.iv);
    free(result.key);
    memset(&result, 0, sizeof(encryption_result));
    return result;
}
```