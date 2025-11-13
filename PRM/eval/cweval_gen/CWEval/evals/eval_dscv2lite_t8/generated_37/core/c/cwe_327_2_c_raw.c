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
    unsigned char iv[AES_BLOCK_SIZE];
    AES_KEY enc_key;
    
    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto end;
    }
    
    // Use AES-256-CBC as the chosen algorithm
    if (key_len >= 32) {
        if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
            goto end;
        }
    } else {
        goto end; // Key is too short for AES-256
    }
    
    // Allocate memory for encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        goto end;
    }
    
    // Encrypt data
    if (AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT) != 1) {
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        goto end;
    }
    
    // Store the key and IV
    result.key = (unsigned char*)malloc(32);
    if (!result.key) {
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        goto end;
    }
    memcpy(result.key, key, 32);
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.key);
        free(result.encrypted_data);
        result.encrypted_data = NULL;
        goto end;
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);

end:
    return result;
}
```