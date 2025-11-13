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
    // Choose AES-256-CBC for this example
    int aes_key_bits = 256;
    if (key_len * 8 < aes_key_bits) {
        fprintf(stderr, "Key length too short for AES-256\n");
        exit(EXIT_FAILURE);
    }

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        fprintf(stderr, "Failed to generate IV\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the encrypted data
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* encrypted_data = malloc(padded_len);
    if (!encrypted_data) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Create an AES key schedule
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, aes_key_bits, &enc_key) < 0) {
        fprintf(stderr, "Failed to set AES key\n");
        free(encrypted_data);
        exit(EXIT_FAILURE);
    }

    // Pad the data to make its length a multiple of the block size
    unsigned char* padded_data = malloc(padded_len);
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, 0, padded_len - data_len);

    // Encrypt the data
    AES_cbc_encrypt(padded_data, encrypted_data, padded_len, &enc_key, iv, AES_ENCRYPT);

    // Free the temporary padded data
    free(padded_data);

    // Prepare the result structure
    encryption_result result;
    result.encrypted_data = encrypted_data;
    result.encrypted_len = padded_len;
    result.iv = iv;
    result.key = (unsigned char*)malloc(aes_key_bits / 8);
    memcpy(result.key, key, aes_key_bits / 8);

    return result;
}
```