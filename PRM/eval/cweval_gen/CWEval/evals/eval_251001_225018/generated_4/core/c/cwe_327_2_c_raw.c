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
    // Choose AES-256-CBC as the encryption algorithm
    const int algo = AES_256_CBC;
    const size_t key_size = 32; // AES-256 key size
    const size_t iv_size = AES_BLOCK_SIZE; // AES block size

    encryption_result result = {NULL, 0, NULL, NULL};

    // Generate a random IV
    if (!RAND_bytes(result.iv = malloc(iv_size), iv_size)) {
        goto cleanup;
    }

    // Initialize the key buffer
    if (!(result.key = malloc(key_size))) {
        goto cleanup;
    }
    memcpy(result.key, key, key_size);

    // Allocate memory for the encrypted data
    result.encrypted_len = data_len + AES_BLOCK_SIZE;
    if (!(result.encrypted_data = malloc(result.encrypted_len))) {
        goto cleanup;
    }

    // Encrypt the data
    AES_KEY aes_key;
    if (algo == AES_256_CBC) {
        if (AES_set_encrypt_key(result.key, 256, &aes_key) != 0) {
            goto cleanup;
        }
        AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, result.iv, AES_ENCRYPT);
    } else {
        // Handle other algorithms if needed
        goto cleanup;
    }

cleanup:
    if (result.encrypted_data) free(result.encrypted_data);
    if (result.iv) free(result.iv);
    if (result.key) free(result.key);

    return result;
}
```