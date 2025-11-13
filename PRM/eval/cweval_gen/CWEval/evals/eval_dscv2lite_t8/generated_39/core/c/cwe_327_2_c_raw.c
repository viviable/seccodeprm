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
    if (data_len == 0 || key_len < 64) {
        return result; // Invalid input
    }

    // Use AES-256-CBC as the encryption algorithm
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char temp_buffer[data_len + AES_BLOCK_SIZE];

    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Failed to generate IV
    }

    // Initialize AES key
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        return result; // Failed to set AES key
    }

    // Pad data to make its length a multiple of the block size
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    memcpy(temp_buffer, data, data_len);
    memset(temp_buffer + data_len, 0x00, padded_len - data_len);

    // Encrypt the data
    AES_cbc_encrypt(temp_buffer, temp_buffer, padded_len, &aes_key, iv, AES_ENCRYPT);

    // Allocate memory for the result
    result.encrypted_data = malloc(padded_len);
    result.encrypted_len = padded_len;
    result.iv = malloc(sizeof(iv));
    result.key = malloc(32); // AES-256 key length

    if (!result.encrypted_data || !result.iv || !result.key) {
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        return result; // Memory allocation failed
    }

    memcpy(result.encrypted_data, temp_buffer, padded_len);
    memcpy(result.iv, iv, sizeof(iv));
    memcpy(result.key, key, 32);

    return result;
}
```