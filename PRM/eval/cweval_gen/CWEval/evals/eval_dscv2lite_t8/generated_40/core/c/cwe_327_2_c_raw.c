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
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();
    if (!cipher) {
        goto end;
    }
    
    // Generate random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto end;
    }
    
    // Allocate memory for the encrypted data
    int encrypted_len = data_len + AES_BLOCK_SIZE;
    result.encrypted_data = (unsigned char*)malloc(encrypted_len);
    if (!result.encrypted_data) {
        goto end;
    }
    
    // Copy IV to result structure
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        goto end;
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);
    
    // Encrypt data
    int len;
    if (1 != EVP_EncryptInit_ex(NULL, cipher, NULL, key, iv)) {
        goto end;
    }
    if (1 != EVP_EncryptUpdate(NULL, result.encrypted_data, &len, data, data_len)) {
        goto end;
    }
    result.encrypted_len = len;
    if (1 != EVP_EncryptFinal_ex(NULL, result.encrypted_data + len, &len)) {
        goto end;
    }
    result.encrypted_len += len;
    
    // Save the key
    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        goto end;
    }
    memcpy(result.key, key, key_len);

end:
    return result;
}
```