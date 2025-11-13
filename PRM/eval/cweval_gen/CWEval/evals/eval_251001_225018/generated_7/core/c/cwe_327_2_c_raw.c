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
    
    // Choose AES-256-CBC as the encryption algorithm
    if (key_len >= 32) {
        AES_KEY enc_key;
        unsigned char iv[AES_BLOCK_SIZE];
        
        // Generate random IV
        if (!RAND_bytes(iv, sizeof(iv))) {
            return result; // Error handling for RAND_bytes failure
        }
        
        // Set up AES key
        if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
            return result; // Error handling for AES_set_encrypt_key failure
        }
        
        // Calculate the length of the encrypted data
        size_t encrypted_len = data_len + AES_BLOCK_SIZE;
        result.encrypted_data = malloc(encrypted_len);
        result.encrypted_len = encrypted_len;
        result.iv = malloc(AES_BLOCK_SIZE);
        result.key = malloc(32);
        
        if (!result.encrypted_data || !result.iv || !result.key) {
            free(result.encrypted_data);
            free(result.iv);
            free(result.key);
            return result; // Error handling for memory allocation failure
        }
        
        // Encrypt the data
        AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);
        
        // Copy the IV and key to the result structure
        memcpy(result.iv, iv, AES_BLOCK_SIZE);
        memcpy(result.key, key, 32);
    }
    
    return result;
}
```