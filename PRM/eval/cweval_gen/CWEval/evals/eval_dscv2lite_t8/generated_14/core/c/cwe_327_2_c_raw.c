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
    // Use AES encryption as an example
    const int aes_key_size = AES_BLOCK_SIZE; // 128 bits
    unsigned char aes_key[aes_key_size];
    memcpy(aes_key, key, aes_key_size);

    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, aes_key_size * 8, &enc_key) < 0) {
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    size_t padded_data_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    unsigned char* padded_data = (unsigned char*)malloc(padded_data_len);
    if (!padded_data) {
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, 0, padded_data_len - data_len);

    unsigned char* encrypted_data = (unsigned char*)malloc(padded_data_len);
    if (!encrypted_data) {
        free(padded_data);
        encryption_result result = {NULL, 0, NULL, NULL};
        return result;
    }

    AES_cbc_encrypt(padded_data, encrypted_data, padded_data_len, &enc_key, iv, AES_ENCRYPT);

    encryption_result result = {
        .encrypted_data = encrypted_data,
        .encrypted_len = padded_data_len,
        .iv = iv,
        .key = aes_key
    };

    free(padded_data);
    return result;
}
```