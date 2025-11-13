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
    unsigned char* encrypted_data = NULL;

    // Generate random IV
    if (!RAND_bytes(iv, sizeof(iv))) {
        return result; // Return empty result on failure
    }

    // Use only the first 32 bytes of the key for AES-128
    if (AES_set_encrypt_key(key, 128, &aes_key) != 0) {
        return result; // Return empty result on failure
    }

    // Allocate memory for encrypted data
    encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!encrypted_data) {
        return result; // Return empty result on failure
    }

    // Pad the data to make it a multiple of the block size
    size_t padded_len = ((data_len + AES_BLOCK_SIZE - 1) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
    memset(encrypted_data + data_len, 0, padded_len - data_len);

    // Encrypt the data
    AES_cbc_encrypt(data, encrypted_data, padded_len, &aes_key, iv, AES_ENCRYPT);

    // Store the results
    result.encrypted_data = encrypted_data;
    result.encrypted_len = padded_len;
    result.iv = iv;
    result.key = (unsigned char*)malloc(AES_BLOCK_SIZE);
    memcpy(result.key, key, AES_BLOCK_SIZE);

    return result;
}
```