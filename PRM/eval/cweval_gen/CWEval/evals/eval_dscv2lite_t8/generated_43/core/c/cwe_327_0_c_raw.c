```c
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/pem.h>

#define HEX_LENGTH SHA256_DIGEST_LENGTH * 2 + 1

char* hash_certificate(const char *cert) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        fprintf(stderr, "Error creating MD context\n");
        return NULL;
    }

    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
        fprintf(stderr, "Error initializing digest\n");
        EVP_MD_CTX_free(mdctx);
        return NULL;
    }

    if (1 != EVP_DigestUpdate(mdctx, cert, strlen(cert))) {
        fprintf(stderr, "Error updating digest\n");
        EVP_MD_CTX_free(mdctx);
        return NULL;
    }

    if (1 != EVP_DigestFinal_ex(mdctx, hash, NULL)) {
        fprintf(stderr, "Error finalizing digest\n");
        EVP_MD_CTX_free(mdctx);
        return NULL;
    }

    EVP_MD_CTX_free(mdctx);

    char *hex = malloc(HEX_LENGTH);
    if (!hex) {
        fprintf(stderr, "Error allocating memory for hex string\n");
        return NULL;
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        snprintf(hex + i * 2, 3, "%02x", hash[i]);
    }
    hex[HEX_LENGTH - 1] = '\0';

    return hex;
}
```