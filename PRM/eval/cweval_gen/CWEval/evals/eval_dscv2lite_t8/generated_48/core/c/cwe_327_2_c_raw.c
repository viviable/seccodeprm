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
    AES_KEY aes_key;
    unsigned char iv[AES_BLOCK_SIZE];

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        goto end;
    }

    // Use the first 32 bytes of the key for AES-128
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        goto end;
    }

    // Calculate the length of the encrypted data
    size_t encrypted_len = data_len + AES_BLOCK_SIZE - data_len % AES_BLOCK_SIZE;

    // Allocate memory for encrypted data
    result.encrypted_data = (unsigned char*)malloc(encrypted_len);
    if (!result.encrypted_data) {
        goto end;
    }
    result.encrypted_len = encrypted_len;

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, encrypted_len, &aes_key, iv, AES_ENCRYPT);

    // Copy the key
    result.key = (unsigned char*)malloc(key_len);
    if (!result.key) {
        free(result.encrypted_data);
        goto end;
    }
    memcpy(result.key, key, key_len);

end:
    return result;
}
```