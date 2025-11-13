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
    encryption_result result;
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }

    // Set up encryption key
    if (AES_set_encrypt_key(key, key_len * 8, &enc_key) < 0) {
        result.encrypted_data = NULL;
        result.encrypted_len = 0;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }

    // Calculate the encrypted data length
    size_t encrypted_len = AES_BLOCK_SIZE + ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    // Allocate memory for the encrypted data
    result.encrypted_data = (unsigned char*)malloc(encrypted_len);
    if (result.encrypted_data == NULL) {
        result.encrypted_len = 0;
        result.iv = NULL;
        result.key = NULL;
        return result;
    }

    // Perform encryption
    int num_rounds;
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Copy the IV and key to the result structure
    result.iv = (unsigned char*)malloc(sizeof(iv));
    memcpy(result.iv, iv, sizeof(iv));

    result.key = (unsigned char*)malloc(key_len);
    memcpy(result.key, key, key_len);

    result.encrypted_len = encrypted_len;

    return result;
}
```