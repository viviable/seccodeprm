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
    // Choose the appropriate AES key size based on the input key length
    int aes_key_bits = 256; // Default to 256-bit
    if (key_len >= 32) aes_key_bits = 256;
    else if (key_len >= 16) aes_key_bits = 192;
    else if (key_len >= 8) aes_key_bits = 128;
    else {
        // Handle error: key too short
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    // Generate IV
    unsigned char iv[AES_BLOCK_SIZE];
    for (int i = 0; i < AES_BLOCK_SIZE; ++i) iv[i] = rand() % 256;

    // Allocate memory for encrypted data
    encryption_result result;
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        result.iv = NULL;
        result.key = NULL;
        return result;
    }

    // Encrypt data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, aes_key_bits, &enc_key) != 0) {
        free(result.encrypted_data);
        result.iv = NULL;
        result.key = NULL;
        return result;
    }
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Copy IV and key to result structure
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    memcpy(result.iv, iv, AES_BLOCK_SIZE);
    result.key = (unsigned char*)malloc(aes_key_bits / 8);
    memcpy(result.key, key, aes_key_bits / 8);

    return result;
}
```