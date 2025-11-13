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
    unsigned char iv[AES_BLOCK_SIZE];
    AES_KEY aes_key;

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Failed to generate IV
    }

    // Use the first 32 bytes of the key as AES key
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        return result; // Failed to set AES key
    }

    // Calculate the padded length
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(padded_len);
    if (!result.encrypted_data) {
        return result; // Memory allocation failed
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, padded_len, &aes_key, iv, AES_ENCRYPT);

    result.encrypted_len = padded_len;
    result.iv = (unsigned char*)malloc(sizeof(iv));
    if (!result.iv) {
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.iv, iv, sizeof(iv));

    result.key = (unsigned char*)malloc(32); // AES-256 key length
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, 32);

    return result;
}
```