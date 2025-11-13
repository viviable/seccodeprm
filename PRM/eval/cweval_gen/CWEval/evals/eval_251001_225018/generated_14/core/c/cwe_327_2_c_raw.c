```c
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/rand.h>
#include <stdlib.h>

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
    
    // Choose AES-128-CBC as the encryption algorithm
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];
    unsigned char *encrypted = NULL;
    
    // Generate random IV
    if (!RAND_bytes(iv, AES_BLOCK_SIZE)) {
        goto cleanup;
    }
    
    // Set up AES key
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        goto cleanup;
    }
    
    // Allocate memory for encrypted data
    encrypted = (unsigned char *)malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted) {
        goto cleanup;
    }
    
    // Encrypt data
    AES_cbc_encrypt(data, encrypted, data_len, &aes_key, iv, AES_ENCRYPT);
    
    // Store results
    result.encrypted_data = encrypted;
    result.encrypted_len = data_len;
    result.iv = iv;
    result.key = (unsigned char *)malloc(16); // AES-128 key length
    memcpy(result.key, key, 16);
    
cleanup:
    if (!result.encrypted_data) {
        free(encrypted);
    }
    if (!result.iv) {
        free(iv);
    }
    
    return result;
}
```