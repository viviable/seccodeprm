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
    if (key_len < AES_BLOCK_SIZE || data_len == 0) {
        return result; // Invalid input
    }

    // Use AES-128 for simplicity and because it's commonly supported
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        return result; // Failed to set up AES key
    }

    // Generate random IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Failed to generate IV
    }

    // Calculate the length of the encrypted data
    size_t encrypted_len = data_len + AES_BLOCK_SIZE - (data_len % AES_BLOCK_SIZE);
    unsigned char* encrypted_data = malloc(encrypted_len);
    if (!encrypted_data) {
        return result; // Memory allocation failed
    }

    // Encrypt the data
    AES_cbc_encrypt(data, encrypted_data, encrypted_len, &aes_key, iv, AES_ENCRYPT);

    // Store the results
    result.encrypted_data = encrypted_data;
    result.encrypted_len = encrypted_len;
    result.iv = iv;
    result.key = (unsigned char*)malloc(AES_BLOCK_SIZE);
    memcpy(result.key, key, AES_BLOCK_SIZE);

    return result;
}
```