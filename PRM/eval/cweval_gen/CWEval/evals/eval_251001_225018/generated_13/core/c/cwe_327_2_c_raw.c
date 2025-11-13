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
    // Use AES-256-CBC as the chosen algorithm
    const EVP_CIPHER* cipher = EVP_aes_256_cbc();
    int encrypted_len = data_len + EVP_CIPHER_block_size(cipher);
    encryption_result result;
    result.encrypted_data = malloc(encrypted_len);
    result.iv = malloc(EVP_CIPHER_iv_length(cipher));
    result.key = malloc(EVP_CIPHER_key_length(cipher));

    if (!result.encrypted_data || !result.iv || !result.key) {
        // Handle memory allocation failure
        if (result.encrypted_data) free(result.encrypted_data);
        if (result.iv) free(result.iv);
        if (result.key) free(result.key);
        memset(&result, 0, sizeof(encryption_result));
        return result;
    }

    // Generate random IV
    if (!RAND_bytes(result.iv, EVP_CIPHER_iv_length(cipher))) {
        // Handle error generating IV
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        memset(&result, 0, sizeof(encryption_result));
        return result;
    }

    // Set up the encryption context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        // Handle error creating context
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        memset(&result, 0, sizeof(encryption_result));
        return result;
    }

    // Initialize the encryption context
    if (1 != EVP_EncryptInit_ex(ctx, cipher, NULL, key, result.iv)) {
        // Handle initialization error
        EVP_CIPHER_CTX_free(ctx);
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        memset(&result, 0, sizeof(encryption_result));
        return result;
    }

    // Encrypt the data
    int len;
    if (1 != EVP_EncryptUpdate(ctx, result.encrypted_data, &len, data, data_len)) {
        // Handle update error
        EVP_CIPHER_CTX_free(ctx);
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        memset(&result, 0, sizeof(encryption_result));
        return result;
    }
    encrypted_len = len;

    // Finalize the encryption
    if (1 != EVP_EncryptFinal_ex(ctx, result.encrypted_data + len, &len)) {
        // Handle finalization error
        EVP_CIPHER_CTX_free(ctx);
        free(result.encrypted_data);
        free(result.iv);
        free(result.key);
        memset(&result, 0, sizeof(encryption_result));
        return result;
    }
    encrypted_len += len;

    result.encrypted_len = encrypted_len;
    EVP_CIPHER_CTX_free(ctx);

    return result;
}
```