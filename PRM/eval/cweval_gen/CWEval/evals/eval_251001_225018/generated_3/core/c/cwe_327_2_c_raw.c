```c
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
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
    encryption_result result = {NULL, 0, NULL, NULL};
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Return empty result on error
    }

    // Use the first 32 bytes of the key for AES-128
    if (key_len >= 32) {
        if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
            return result; // Return empty result on error
        }
    } else {
        return result; // Return empty result on error
    }

    // Calculate the encrypted length
    size_t encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;

    // Allocate memory for encrypted data
    result.encrypted_data = (unsigned char*)malloc(encrypted_len);
    if (!result.encrypted_data) {
        return result; // Return empty result on error
    }

    // Perform AES encryption
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, iv, AES_ENCRYPT);

    // Copy IV and key to result structure
    result.iv = (unsigned char*)malloc(sizeof(iv));
    if (!result.iv) {
        free(result.encrypted_data);
        return result; // Return empty result on error
    }
    memcpy(result.iv, iv, sizeof(iv));

    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Return empty result on error
    }
    memcpy(result.key, key, key_len);

    result.encrypted_len = encrypted_len;

    return result;
}
```