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
    
    // Use AES-256-CBC for encryption as it requires at least 32 bytes of key
    if (key_len >= 32) {
        AES_KEY enc_key;
        unsigned char iv[AES_BLOCK_SIZE];
        
        // Generate random IV
        if (!RAND_bytes(iv, sizeof(iv))) {
            return result; // Failed to generate IV
        }
        
        // Set up AES encryption key
        if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
            return result; // Failed to set encryption key
        }
        
        // Calculate the length of the encrypted data
        size_t encrypted_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
        
        // Allocate memory for the encrypted data
        result.encrypted_data = malloc(encrypted_len);
        if (!result.encrypted_data) {
            return result; // Memory allocation failed
        }
        
        // Encrypt the data
        AES_cbc_encrypt(data, result.encrypted_data, encrypted_len, &enc_key, iv, AES_ENCRYPT);
        
        // Store the IV and key
        result.iv = malloc(sizeof(iv));
        memcpy(result.iv, iv, sizeof(iv));
        
        result.key = malloc(32); // AES-256 key length
        memcpy(result.key, key, 32);
        
        result.encrypted_len = encrypted_len;
    } else {
        // Handle cases where key length is less than 32 bytes
        // For simplicity, we will not handle DES in this example
        return result; // Key length too short
    }
    
    return result;
}
```