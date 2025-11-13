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
    // Determine the effective key length for AES (128, 192, or 256 bits)
    int effective_key_len = key_len >= 32 ? 32 : (key_len >= 24 ? 24 : 16);

    // Allocate memory for the result structure
    encryption_result result;
    result.encrypted_data = malloc(data_len + AES_BLOCK_SIZE);
    if (!result.encrypted_data) {
        return result; // Return empty result on allocation failure
    }

    // Initialize the IV and copy it to the result
    result.iv = malloc(AES_BLOCK_SIZE);
    if (!result.iv) {
        free(result.encrypted_data);
        return result; // Return empty result on allocation failure
    }
    randombytes_buf(result.iv, AES_BLOCK_SIZE); // Fill IV with random bytes

    // Set up the AES key schedule
    AES_KEY aes_key;
    if (AES_set_encrypt_key(key, effective_key_len * 8, &aes_key) != 0) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Return empty result on key setup failure
    }

    // Encrypt the data
    AES_cbc_encrypt(data, result.encrypted_data, data_len, &aes_key, result.iv, AES_ENCRYPT);
    result.encrypted_len = data_len;

    // Store the key used for encryption
    result.key = malloc(effective_key_len);
    if (!result.key) {
        free(result.encrypted_data);
        free(result.iv);
        return result; // Return empty result on allocation failure
    }
    memcpy(result.key, key, effective_key_len);

    return result;
}
```
This code snippet includes the necessary OpenSSL headers and defines the `encrypt_data` function as specified. It handles memory allocation, initialization vectors, and AES encryption in CBC mode. Error handling is included for memory allocation failures and AES key setup errors.