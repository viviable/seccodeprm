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
    // For simplicity, we will use AES-128-CBC
    if (key_len < AES_BLOCK_SIZE) {
        fprintf(stderr, "Key length is too short for AES-128\n");
        exit(EXIT_FAILURE);
    }

    encryption_result result = {0};
    result.key = malloc(AES_BLOCK_SIZE);
    memcpy(result.key, key, AES_BLOCK_SIZE);

    // Generate random IV
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        fprintf(stderr, "Failed to generate IV\n");
        free(result.key);
        exit(EXIT_FAILURE);
    }

    // Encrypt data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, 128, &enc_key) != 0) {
        fprintf(stderr, "Failed to set up AES encryption key\n");
        free(result.key);
        free(result.iv);
        exit(EXIT_FAILURE);
    }

    // Calculate the length of the encrypted data
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = malloc(padded_len);
    result.encrypted_len = padded_len;

    // Pad data
    unsigned char* padded_data = malloc(data_len + AES_BLOCK_SIZE);
    memcpy(padded_data, data, data_len);
    for (size_t i = data_len; i < padded_len; ++i) {
        padded_data[i] = AES_BLOCK_SIZE - (data_len % AES_BLOCK_SIZE);
    }

    // Perform encryption
    AES_cbc_encrypt(padded_data, result.encrypted_data, padded_len, &enc_key, result.iv, AES_ENCRYPT);

    free(padded_data);

    return result;
}
```