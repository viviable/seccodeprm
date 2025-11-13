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
    
    // Ensure the key length is at least 32 bytes (AES-256)
    if (key_len < 32) {
        return result;
    }
    
    // Generate IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result;
    }
    
    // Create an AES key schedule
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, 256, &aes_key) != 0) {
        return result;
    }
    
    // Allocate memory for the encrypted data
    size_t encrypted_len = data_len + AES_BLOCK_SIZE; // Padding might be needed
    unsigned char* encrypted_data = malloc(encrypted_len);
    if (!encrypted_data) {
        return result;
    }
    
    // Encrypt the data
    int len;
    AES_cbc_encrypt(data, encrypted_data, data_len, &aes_key, iv, AES_ENCRYPT);
    
    // Update the result structure
    result.encrypted_data = encrypted_data;
    result.encrypted_len = encrypted_len;
    result.iv = iv;
    result.key = (unsigned char*)malloc(32); // Copy only the first 32 bytes of the key
    if (result.key) {
        memcpy(result.key, key, 32);
    }
    
    return result;
}
```