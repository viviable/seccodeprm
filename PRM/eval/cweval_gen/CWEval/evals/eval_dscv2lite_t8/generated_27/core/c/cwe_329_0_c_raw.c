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
    AES_KEY enc_key;
    unsigned char iv[AES_BLOCK_SIZE];

    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        // Handle error
        exit(1);
    }

    // Set up the AES encryption key
    if (AES_set_encrypt_key(key, 256, &enc_key) < 0) {
        // Handle error
        exit(1);
    }

    // Calculate the encrypted data length
    result.encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Store the key and IV
    result.key = (unsigned char*)malloc(key_len);
    memcpy(result.key, key, key_len);
    result.iv = (unsigned char*)malloc(sizeof(iv));
    memcpy(result.iv, iv, sizeof(iv));

    return result;
}
```