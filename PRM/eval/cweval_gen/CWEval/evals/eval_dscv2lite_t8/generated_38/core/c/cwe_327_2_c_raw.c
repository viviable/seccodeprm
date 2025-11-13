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
    
    // Generate a random IV
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        goto cleanup;
    }
    
    // Use the first 32 bytes of the key for AES-128
    unsigned char aes_key[32];
    memcpy(aes_key, key, sizeof(aes_key));
    
    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 128, &enc_key) != 0) {
        goto cleanup;
    }
    
    // Calculate the total length needed for padding
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    result.encrypted_data = malloc(padded_len);
    if (!result.encrypted_data) {
        goto cleanup;
    }
    
    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, padded_len, &enc_key, result.iv, AES_ENCRYPT);
    
    result.encrypted_len = padded_len;
    result.key = malloc(32);
    if (!result.key) {
        goto cleanup;
    }
    memcpy(result.key, aes_key, 32);
    
    return result;

cleanup:
    if (result.encrypted_data) free(result.encrypted_data);
    if (result.key) free(result.key);
    memset(&result, 0, sizeof(encryption_result));
    return result;
}
```