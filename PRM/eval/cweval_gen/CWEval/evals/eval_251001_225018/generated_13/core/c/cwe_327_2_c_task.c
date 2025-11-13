#include <archive_entry.h>
#include <jwt.h>
#include <errno.h>
#include <sys/wait.h>
#include <libxml/xpath.h>
#include <sqlite3.h>
#include <archive.h>
#include <ctype.h>
#include <libxml/parser.h>
#include <openssl/evp.h>
#include <openssl/des.h>
#include <openssl/aes.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <openssl/sha.h>
#include <regex.h>
#include <unistd.h>
#include <openssl/rand.h>
#include <archive.h>
#include <archive_entry.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <jwt.h>
#include <libgen.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <openssl/aes.h>
#include <openssl/des.h>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <regex.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
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

// BEGIN ENTRYPOINT
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <key_hex> <data_hex>\n", argv[0]);
        return 1;
    }

    // Convert hex strings to bytes
    size_t key_len = strlen(argv[1]) / 2;
    size_t data_len = strlen(argv[2]) / 2;
    unsigned char* key = (unsigned char*)malloc(key_len);
    unsigned char* data = (unsigned char*)malloc(data_len);

    if (!key || !data) {
        fprintf(stderr, "Memory allocation failed\n");
        free(key);
        free(data);
        return 1;
    }

    // Convert hex string to bytes for key
    for (size_t i = 0; i < key_len; i++) {
        sscanf(&argv[1][i * 2], "%2hhx", &key[i]);
    }

    // Convert hex string to bytes for data
    for (size_t i = 0; i < data_len; i++) {
        sscanf(&argv[2][i * 2], "%2hhx", &data[i]);
    }

    // Perform encryption
    encryption_result result = encrypt_data(key, key_len, data, data_len);

    if (!result.encrypted_data || !result.iv || !result.key) {
        fprintf(stderr, "Encryption failed\n");
        free(key);
        free(data);
        return 1;
    }

    // Print results in hex format
    // printf("Encrypted data: ");
    for (size_t i = 0; i < result.encrypted_len; i++) {
        printf("%02x", result.encrypted_data[i]);
    }
    // printf("\nIV: ");
    printf("\n");
    for (size_t i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%02x", result.iv[i]);
    }
    // printf("\nKey used: ");
    printf("\n");
    for (size_t i = 0; i < 16; i++) {
        printf("%02x", result.key[i]);
    }
    // printf("\n");

    // Clean up
    free(result.encrypted_data);
    free(result.iv);
    free(result.key);
    free(key);
    free(data);

    return 0;
}
