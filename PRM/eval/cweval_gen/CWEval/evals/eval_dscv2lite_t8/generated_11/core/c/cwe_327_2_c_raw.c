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
    unsigned char* encrypted = NULL;

    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto cleanup;
    }

    // Use the first 32 bytes of the key as AES key
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        goto cleanup;
    }

    // Calculate the encrypted data length
    size_t encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    encrypted = malloc(encrypted_len);
    if (!encrypted) {
        goto cleanup;
    }

    // Encrypt the data
    AES_cbc_encrypt(data, encrypted, data_len, &aes_key, iv, AES_ENCRYPT);

    // Set the result
    result.encrypted_data = encrypted;
    result.encrypted_len = encrypted_len;
    result.iv = memcpy(malloc(sizeof(iv)), iv, sizeof(iv));
    result.key = memcpy(malloc(16), key, 16); // Using the first 16 bytes of the key

cleanup:
    free(encrypted);
    return result;
}
```