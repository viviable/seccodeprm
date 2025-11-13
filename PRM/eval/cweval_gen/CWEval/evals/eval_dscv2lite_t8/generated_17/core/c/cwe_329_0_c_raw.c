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
    // Ensure we have at least 16 bytes for the key
    if (key_len < 16) {
        return (encryption_result){NULL, 0, NULL, NULL};
    }

    // Use the first 16 bytes as the key for AES-128
    unsigned char aes_key[16];
    memcpy(aes_key, key, 16);

    // Generate a random IV
    unsigned char iv[AES_BLOCK_SIZE];
    OpenSSL_add_all_algorithms();
    RAND_bytes(iv, sizeof(iv));

    // Allocate memory for the encrypted data
    size_t encrypted_len = data_len + AES_BLOCK_SIZE;
    unsigned char* encrypted_data = (unsigned char*)malloc(encrypted_len);
    if (!encrypted_data) {
        return (encryption_result){NULL, 0, NULL, NULL};
    }

    // Encrypt the data
    AES_KEY enc_key;
    if (AES_set_encrypt_key(aes_key, 128, &enc_key) != 0) {
        free(encrypted_data);
        return (encryption_result){NULL, 0, NULL, NULL};
    }
    AES_cbc_encrypt(data, encrypted_data, data_len, &enc_key, iv, AES_ENCRYPT);

    // Prepare the result structure
    encryption_result result;
    result.encrypted_data = encrypted_data;
    result.encrypted_len = encrypted_len;
    result.iv = (unsigned char*)malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(encrypted_data);
        return (encryption_result){NULL, 0, NULL, NULL};
    }
    memcpy(result.iv, iv, AES_BLOCK_SIZE);
    result.key = (unsigned char*)malloc(16);
    if (!result.key) {
        free(encrypted_data);
        free(result.iv);
        return (encryption_result){NULL, 0, NULL, NULL};
    }
    memcpy(result.key, aes_key, 16);

    return result;
}
```