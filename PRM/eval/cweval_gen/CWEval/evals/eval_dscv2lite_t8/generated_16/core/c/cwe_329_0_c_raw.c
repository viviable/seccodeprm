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
    // Choose a key length that OpenSSL supports (128, 192, or 256 bits)
    int aes_key_bits = key_len * 8;
    if (aes_key_bits != 128 && aes_key_bits != 192 && aes_key_bits != 256) {
        abort(); // Unsupported key length
    }

    // Generate an IV
    unsigned char iv[AES_BLOCK_SIZE];
    if (!RAND_bytes(iv, sizeof(iv))) {
        abort(); // Failed to generate random IV
    }

    // Create a buffer for the encrypted data
    size_t encrypted_len = data_len + AES_BLOCK_SIZE; // Padding might be needed
    unsigned char* encrypted_data = malloc(encrypted_len);
    if (!encrypted_data) {
        abort(); // Memory allocation failed
    }

    // Set up the AES context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        free(encrypted_data);
        abort(); // Context creation failed
    }

    // Initialize the encryption operation
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv)) {
        EVP_CIPHER_CTX_free(ctx);
        free(encrypted_data);
        abort(); // Encryption initialization failed
    }

    // Perform the encryption
    int len;
    if (!EVP_EncryptUpdate(ctx, encrypted_data, &len, data, data_len)) {
        EVP_CIPHER_CTX_free(ctx);
        free(encrypted_data);
        abort(); // Encryption update failed
    }
    size_t written = len;

    // Finalize the encryption
    if (!EVP_EncryptFinal_ex(ctx, encrypted_data + len, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        free(encrypted_data);
        abort(); // Encryption finalization failed
    }
    written += len;

    // Clean up
    EVP_CIPHER_CTX_free(ctx);

    // Return the result
    encryption_result result;
    result.encrypted_data = encrypted_data;
    result.encrypted_len = written;
    result.iv = iv;
    result.key = (unsigned char*)key; // Note: Returning the original key pointer

    return result;
}
```