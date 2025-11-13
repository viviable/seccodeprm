```c
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
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
    encryption_result result = {NULL, 0, NULL, NULL};
    if (data_len == 0 || key_len < 64) {
        return result; // Invalid input
    }

    // Choose AES-256-CBC as the encryption algorithm
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();
    int key_size = EVP_CIPHER_key_length(cipher);
    int block_size = EVP_CIPHER_block_length(cipher);

    // Allocate memory for the IV and key
    result.iv = malloc(block_size);
    result.key = malloc(key_size);
    if (!result.iv || !result.key) {
        goto cleanup;
    }
    memcpy(result.key, key, key_size);

    // Generate a random IV
    if (!RAND_bytes(result.iv, block_size)) {
        goto cleanup;
    }

    // Calculate the total length of the encrypted data
    size_t total_len = data_len + block_size - (data_len % block_size);
    if (total_len % block_size != 0) {
        total_len += block_size;
    }
    result.encrypted_data = malloc(total_len);
    if (!result.encrypted_data) {
        goto cleanup;
    }
    memset(result.encrypted_data, 0, total_len);

    // Pad the data to make it a multiple of the block size
    unsigned char* padded_data = malloc(data_len + block_size);
    if (!padded_data) {
        goto cleanup;
    }
    memcpy(padded_data, data, data_len);
    while (data_len % block_size != 0) {
        padded_data[data_len++] = block_size - (data_len % block_size);
    }

    // Encrypt the data
    int encrypted_len;
    EVP_EncryptInit_ex(NULL, cipher, NULL, result.key, result.iv);
    EVP_EncryptUpdate(NULL, result.encrypted_data, &encrypted_len, padded_data, data_len);
    int final_len;
    EVP_EncryptFinal_ex(NULL, result.encrypted_data + encrypted_len, &final_len);
    encrypted_len += final_len;
    result.encrypted_len = encrypted_len;

cleanup:
    free(padded_data);
    if (!result.encrypted_data && result.key) {
        free(result.key);
    }
    if (!result.encrypted_data && !result.key && result.iv) {
        free(result.iv);
    }
    return result;
}
```