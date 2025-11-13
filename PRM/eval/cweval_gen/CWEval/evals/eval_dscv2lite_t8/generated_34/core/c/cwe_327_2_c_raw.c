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
    // Use AES-256-CBC as the chosen algorithm
    const int key_size = 32; // AES-256 key size in bytes
    const int iv_size = AES_BLOCK_SIZE; // IV size for AES

    encryption_result result;
    result.key = malloc(key_size);
    if (!result.key) {
        return result; // Handle memory allocation failure
    }
    memcpy(result.key, key, key_size);

    result.iv = malloc(iv_size);
    if (!result.iv) {
        free(result.key);
        return result; // Handle memory allocation failure
    }

    if (RAND_bytes(result.iv, iv_size) != 1) {
        free(result.key);
        free(result.iv);
        return result; // Handle random number generation failure
    }

    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE); // Padding may be needed
    if (!result.encrypted_data) {
        free(result.key);
        free(result.iv);
        return result; // Handle memory allocation failure
    }

    AES_KEY enc_key;
    if (AES_set_encrypt_key(result.key, key_size * 8, &enc_key) != 0) {
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        return result; // Handle key setup failure
    }

    int pad_len = AES_BLOCK_SIZE - (data_len % AES_BLOCK_SIZE);
    unsigned char* padded_data = malloc(data_len + pad_len);
    if (!padded_data) {
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        return result; // Handle memory allocation failure
    }
    memcpy(padded_data, data, data_len);
    memset(padded_data + data_len, pad_len, pad_len);

    AES_cbc_encrypt(padded_data, result.encrypted_data, data_len + pad_len, &enc_key, result.iv, AES_ENCRYPT);

    result.encrypted_len = data_len + pad_len;

    free(padded_data);

    return result;
}
```