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
    // Ensure the key length is at least 16 bytes for AES-128
    if (key_len < 16) {
        return (encryption_result){NULL, 0, NULL, NULL};
    }

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        return (encryption_result){NULL, 0, NULL, NULL};
    }

    // Determine the key length for AES
    int aes_key_len = key_len >= 32 ? 32 : key_len >= 24 ? 24 : 16;

    // Allocate memory for the encrypted data
    size_t encrypted_len = AES_BLOCK_SIZE + ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE * AES_BLOCK_SIZE);
    unsigned char* encrypted_data = malloc(encrypted_len);
    if (!encrypted_data) {
        return (encryption_result){NULL, 0, NULL, NULL};
    }

    // Encrypt the data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, aes_key_len * 8, &enc_key) != 0) {
        free(encrypted_data);
        return (encryption_result){NULL, 0, NULL, NULL};
    }
    AES_cbc_encrypt(data, encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Prepare the result structure
    encryption_result result = {
        .encrypted_data = encrypted_data,
        .encrypted_len = encrypted_len,
        .iv = (unsigned char*)malloc(sizeof(iv));
        .key = (unsigned char*)malloc(aes_key_len)
    };
    if (!result.iv || !result.key) {
        free(encrypted_data);
        free(result.iv);
        free(result.key);
        return (encryption_result){NULL, 0, NULL, NULL};
    }
    memcpy(result.iv, iv, sizeof(iv));
    memcpy(result.key, key, aes_key_len);

    return result;
}
```