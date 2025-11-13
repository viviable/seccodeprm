```c
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_HASH_LENGTH 128

char* hash_certificate(const char *cert) {
    EVP_MD_CTX *mdctx;
    unsigned char hash[MAX_HASH_LENGTH];
    unsigned int len;

    // Create the message digest context
    mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        fprintf(stderr, "Error creating MD context\n");
        return NULL;
    }

    // Initialize the digest context with SHA-256
    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL) != 1) {
        fprintf(stderr, "Error initializing Digest\n");
        EVP_MD_CTX_free(mdctx);
        return NULL;
    }

    // Update the digest with the certificate data
    if (EVP_DigestUpdate(mdctx, cert, strlen(cert)) != 1) {
        fprintf(stderr, "Error updating Digest\n");
        EVP_MD_CTX_free(mdctx);
        return NULL;
    }

    // Finalize the digest computation
    if (EVP_DigestFinal_ex(mdctx, hash, &len) != 1) {
        fprintf(stderr, "Error finalizing Digest\n");
        EVP_MD_CTX_free(mdctx);
        return NULL;
    }

    // Clean up the digest context
    EVP_MD_CTX_free(mdctx);

    // Convert the hash to a hexadecimal string
    char *hash_hex = malloc(2 * len + 1);
    if (!hash_hex) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }
    for (unsigned int i = 0; i < len; ++i) {
        sprintf(hash_hex + 2 * i, "%02x", hash[i]);
    }

    return hash_hex;
}
```