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
    if (data_len == 0 || key_len < AES_BLOCK_SIZE) {
        return result; // Early return for invalid input
    }

    // Generate random IV
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        return result; // Memory allocation failed
    }
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        free(result.iv);
        return result; // Failed to generate IV
    }

    // Use the first AES_BLOCK_SIZE bytes of the key as the AES key
    result.key = malloc(AES_BLOCK_SIZE);
    if (!result.key) {
        free(result.iv);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, AES_BLOCK_SIZE);

    // Encrypt the data using AES-128-CBC
    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key, 128, &enc_key) != 0) {
        free(result.iv);
        free(result.key);
        return result; // Failed to set up AES key
    }

    // Calculate the total length needed for the encrypted data
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = malloc(padded_len);
    if (!result.encrypted_data) {
        free(result.iv);
        free(result.key);
        return result; // Memory allocation failed
    }

    // Pad the data to make its length a multiple of the block size
    unsigned char* padded_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!padded_data) {
        free(result.iv);
        free(result.key);
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(padded_data, data, data_len);
    while (data_len < padded_len) {
        padded_data[data_len++] = 0x00; // PKCS#7 padding
    }

    // Perform the encryption
    AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, &enc_key, result.iv, AES_ENCRYPT);

    // Free temporary memory
    free(padded_data);

    // Set the actual length of the encrypted data
    result.encrypted_len = padded_len;

    return result;
}
```