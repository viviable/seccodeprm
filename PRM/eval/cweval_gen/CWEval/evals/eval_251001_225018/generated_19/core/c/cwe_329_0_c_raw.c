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
    result.encrypted_data = NULL;
    result.encrypted_len = 0;
    result.iv = NULL;
    result.key = NULL;

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Error handling for RAND_bytes failure
    }

    // Determine the key length for AES
    AES_KEY aes_key;
    int key_bits;
    if (key_len >= 32) {
        key_bits = 256;
    } else if (key_len >= 24) {
        key_bits = 192;
    } else if (key_len >= 16) {
        key_bits = 128;
    } else {
        return result; // Key length too short
    }

    // Set up AES key
    if (AES_set_encrypt_key(key, key_bits, &aes_key) < 0) {
        return result; // Error handling for AES_set_encrypt_key failure
    }

    // Allocate memory for encrypted data
    size_t encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(encrypted_len);
    if (!result.encrypted_data) {
        return result; // Memory allocation failure
    }
    result.encrypted_len = encrypted_len;

    // Encrypt data
    if (AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, iv, AES_ENCRYPT) != 0) {
        free(result.encrypted_data);
        return result; // Error handling for AES_cbc_encrypt failure
    }

    // Duplicate the IV and key for returning
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        return result; // Memory allocation failure
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

    result.key = (unsigned char*)malloc(key_bits / 8);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Memory allocation failure
    }
    memcpy(result.key, key, key_bits / 8);

    return result;
}
```