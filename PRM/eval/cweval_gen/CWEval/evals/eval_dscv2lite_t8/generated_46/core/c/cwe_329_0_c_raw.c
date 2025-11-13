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
    encryption_result result = {NULL, 0, NULL, NULL};
    
    // Ensure the key length is at least AES_BLOCK_SIZE (16 bytes)
    if (key_len < AES_BLOCK_SIZE) {
        return result; // Return an empty result if key is too short
    }
    
    // Allocate memory for the IV
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        return result; // Return an empty result on allocation failure
    }
    
    // Initialize IV with random bytes
    if (!RAND_bytes(result.iv, AES_BLOCK_SIZE)) {
        free(result.iv);
        return result; // Return an empty result on failed IV generation
    }
    
    // Allocate memory for the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = malloc(result.encrypted_len);
    if (!result.encrypted_data) {
        free(result.iv);
        return result; // Return an empty result on allocation failure
    }
    
    // Set up AES key schedule
    AES_KEY enc_key;
    if (AES_set_encrypt_key(key, key_len * 8, &enc_key) != 0) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Return an empty result on key setup failure
    }
    
    // Perform AES encryption in CBC mode
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, result.iv, AES_ENCRYPT);
    
    // Duplicate the key for returning
    result.key = malloc(key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Return an empty result on allocation failure
    }
    memcpy(result.key, key, key_len);
    
    return result;
}
```