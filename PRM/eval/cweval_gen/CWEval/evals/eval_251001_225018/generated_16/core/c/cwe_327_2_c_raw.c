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
    if (key_len < AES_BLOCK_SIZE || data_len == 0) {
        return result; // Invalid input
    }

    unsigned char aes_key[AES_BLOCK_SIZE];
    memcpy(aes_key, key, AES_BLOCK_SIZE);

    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        return result; // Failed to generate IV
    }

    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 128, &enc_key) != 0) {
        return result; // Failed to set encryption key
    }

    result.iv = malloc(AES_BLOCK_SIZE);
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

    result.encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = malloc(result.encrypted_len);

    int num_blocks = result.encrypted_len / AES_BLOCK_SIZE;
    for (int i = 0; i < num_blocks; ++i) {
        AES_cbc_encrypt(data + i * AES_BLOCK_SIZE, result.encrypted_data + i * AES_BLOCK_SIZE, AES_BLOCK_SIZE, &enc_key, iv, AES_ENCRYPT);
    }

    result.key = malloc(AES_BLOCK_SIZE);
    memcpy(result.key, aes_key, AES_BLOCK_SIZE);

    return result;
}
```