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
    const int algo = EVP_aes_256_cbc();
    const int key_size = EVP_CIPHER_key_length(algo);
    const int iv_size = EVP_CIPHER_iv_length(algo);

    // Allocate memory for the result
    encryption_result result;
    result.key = malloc(key_size);
    memcpy(result.key, key, key_size);
    result.iv = malloc(iv_size);
    result.encrypted_data = malloc(data_len + EVP_CIPHER_block_size(algo));

    // Generate random IV
    if (!RAND_bytes(result.iv, iv_size)) {
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        abort(); // Handle error appropriately
    }

    // Initialize cipher context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        abort(); // Handle error appropriately
    }

    // Encrypt the data
    if (1 != EVP_EncryptInit_ex(ctx, algo, NULL, result.key, result.iv)) {
        EVP_CIPHER_CTX_free(ctx);
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        abort(); // Handle error appropriately
    }

    int len;
    if (1 != EVP_EncryptUpdate(ctx, result.encrypted_data, &len, data, data_len)) {
        EVP_CIPHER_CTX_free(ctx);
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        abort(); // Handle error appropriately
    }
    result.encrypted_len = len;

    int final_len;
    if (1 != EVP_EncryptFinal_ex(ctx, result.encrypted_data + len, &final_len)) {
        EVP_CIPHER_CTX_free(ctx);
        free(result.key);
        free(result.iv);
        free(result.encrypted_data);
        abort(); // Handle error appropriately
    }
    result.encrypted_len += final_len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    return result;
}
```