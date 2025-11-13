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

    // Initialize variables
    encryption_result result = {NULL, 0, NULL, NULL};
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];

    // Set up the IV and encryption key
    memcpy(iv, key + key_len - AES_BLOCK_SIZE, AES_BLOCK_SIZE);
    AES_set_encrypt_key(key, 128, &enc_key);

    // Calculate the padded data length
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* padded_data = (unsigned char*)malloc(padded_len);
    if (!padded_data) {
        return result;
    }
    memcpy(padded_data, data, data_len);
    // Pad the data with PKCS#7 padding
    for (size_t i = data_len; i < padded_len; ++i) {
        padded_data[i] = padded_len - data_len;
    }

    // Allocate memory for the encrypted data
    result.encrypted_data = (unsigned char*)malloc(padded_len);
    if (!result.encrypted_data) {
        free(padded_data);
        return result;
    }

    // Perform the encryption
    AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, &enc_key, iv, AES_ENCRYPT);

    // Free the temporary data
    free(padded_data);

    // Set the lengths and keys
    result.encrypted_len = padded_len;
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        return result;
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        free(result.iv);
        free(result.encrypted_data);
        return result;
    }
    memcpy(result.key, key, key_len);

    return result;
}
```