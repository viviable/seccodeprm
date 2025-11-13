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
    // Choose a suitable AES key length based on the provided key length
    int aes_key_bits = 0;
    if (key_len >= 32) aes_key_bits = 256;
    else if (key_len >= 16) aes_key_bits = 192;
    else aes_key_bits = 128;

    // Ensure the data length is a multiple of the block size
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* padded_data = (unsigned char*)malloc(padded_len);
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, 0, padded_len - data_len);

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    AES_random_key(iv);

    // Allocate memory for the encrypted data
    unsigned char* encrypted_data = (unsigned char*)malloc(padded_len);
    AES_KEY enc_key;
    AES_set_encrypt_key(key, aes_key_bits, &enc_key);

    // Perform the encryption
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