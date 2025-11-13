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
            goto end;
        }
        
        // Initialize the encryption key
        if (AES_set_encrypt_key(key, 256, &enc_key) != 0) {
            goto end;
        }
        
        // Allocate memory for the encrypted data
        result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
        if (!result.encrypted_data) {
            goto end;
        }
        
        // Encrypt the data
        AES_cbc_encrypt(data, result.encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);
        
        // Store the length of the encrypted data
        result.encrypted_len = data_len + AES_BLOCK_SIZE;
        
        // Store the IV and key
        result.iv = malloc(AES_BLOCK_SIZE);
        memcpy(result.iv, iv, AES_BLOCK_SIZE);
        
        result.key = malloc(32);
        memcpy(result.key, key, 32);
    }

end:
    return result;
}
```