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
    unsigned char iv[AES_BLOCK_SIZE];
    AES_KEY enc_key;
    
    // Generate a random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Error generating IV
    }
    
    // Use the first 32 bytes of the key as AES-128 key
    if (AES_set_encrypt_key(key, 128, &enc_key) != 0) {
        return result; // Error setting AES key
    }
    
    // Allocate memory for encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        return result; // Memory allocation failed
    }
    
    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);
    
    // Store the IV and key
    result.iv = malloc(sizeof(iv));
    if (!result.iv) {
        free(result.encrypted_data);
        return result; // Memory allocation failed
    }
    memcpy(result.iv, iv, sizeof(iv));
    
    result.key = malloc(16); // AES-128 key length
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Memory allocation failed
    }
    memcpy(result.key, key, 16);
    
    return result;
}
```