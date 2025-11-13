```c
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#define HASH_SIZE EVP_MAX_MD_SIZE

char* hash_certificate(const char *cert) {
    unsigned char hash[HASH_SIZE];
    unsigned int hash_len;
    EVP_MD_CTX *mdctx;

    // Create new message digest context
    mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        fprintf(stderr, "Failed to create MD context\n");
        return NULL;
    }

    // Initialize message digest context with SHA-256
    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
        fprintf(stderr, "Failed to initialize digest\n");
        EVP_MD_CTX_free(mdctx);
        return NULL;
    }

    // Update the digest with the certificate data
    if (1 != EVP_DigestUpdate(mdctx, cert, strlen(cert))) {
        fprintf(stderr, "Failed to update digest\n");
        EVP_MD_CTX_free(mdctx);
        return NULL;
    }

    // Finalize the digest computation
    if (1 != EVP_DigestFinal_ex(mdctx, hash, &hash_len)) {
        fprintf(stderr, "Failed to finalize digest\n");
        EVP_MD_CTX_free(mdctx);
        return NULL;
    }

    // Clean up
    EVP_MD_CTX_free(mdctx);

    // Convert hash to hexadecimal string
    char *hex_string = malloc(hash_len * 2 + 1);
    if (!hex_string) {
        fprintf(stderr, "Failed to allocate memory for hex string\n");
        return NULL;
    }
    for (unsigned int i = 0; i < hash_len; ++i) {
        sprintf(hex_string + i * 2, "%02x", hash[i]);
    }

    return hex_string;
}
```