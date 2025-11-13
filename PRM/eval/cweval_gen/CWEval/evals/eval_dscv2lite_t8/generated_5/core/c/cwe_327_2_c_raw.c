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
        
        // Generate a random IV
        if (!RAND_bytes(iv, sizeof(iv))) {
            goto cleanup;
        }
        
        // Initialize the AES key
        if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
            goto cleanup;
        }
        
        // Calculate the encrypted data length
        size_t encrypted_len = AES_BLOCK_SIZE + ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE * AES_BLOCK_SIZE);
        
        // Allocate memory for the encrypted data
        result.encrypted_data = (unsigned char*)malloc(encrypted_len);
        if (!result.encrypted_data) {
            goto cleanup;
        }
        
        // Perform the encryption
        AES_cbc_encrypt(data, result.encrypted_data, encrypted_len, &enc_key, iv, AES_ENCRYPT);
        
        // Store the IV and key
        result.iv = (unsigned char*)malloc(sizeof(iv));
        memcpy(result.iv, iv, sizeof(iv));
        result.key = (unsigned char*)malloc(32);
        memcpy(result.key, key, 32);
        
        // Set the encrypted length
        result.encrypted_len = encrypted_len;
    }

cleanup:
    return result;
}
```