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
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Failed to generate IV
    }

    // Use the first 32 bytes of the key for AES-128
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        return result; // Failed to set AES key
    }

    // Calculate the total encrypted length
    result.encrypted_len = data_len + AES_BLOCK_SIZE - data_len % AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);
    result.key = (unsigned char*)malloc(16); // AES-128 key length

    if (!result.encrypted_data || !result.key) {
        free(result.encrypted_data);
        free(result.key);
        return result; // Memory allocation failed
    }

    // Pad the data to be a multiple of the block size
    unsigned char* padded_data = (unsigned char*)malloc(data_len + AES_BLOCK_SIZE);
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, 0x00, AES_BLOCK_SIZE - data_len % AES_BLOCK_SIZE);

    // Encrypt the data
    AES_cbc_encrypt(padded_data, result.encrypted_data, result.encrypted_len, &aes_key, iv, AES_ENCRYPT);

    // Copy the key to the result structure
    memcpy(result.key, key, 16);

    // Free temporary buffer
    free(padded_data);

    // Return the result
    return result;
}
```