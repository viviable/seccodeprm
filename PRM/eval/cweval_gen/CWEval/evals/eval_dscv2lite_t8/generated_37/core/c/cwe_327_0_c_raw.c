```c
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

char* hash_certificate(const char *cert) {
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

    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;

    if (1 != EVP_DigestFinal_ex(mdctx, md_value, &md_len)) {
        fprintf(stderr, "Error finalizing digest\n");
        EVP_MD_CTX_free(mdctx);
        return NULL;
    }

    EVP_MD_CTX_free(mdctx);

    // Convert the hash to a hexadecimal string
    char *hash_hex = (char *)malloc((md_len * 2 + 1) * sizeof(char));
    if (!hash_hex) {
        fprintf(stderr, "Error allocating memory for hex string\n");
        return NULL;
    }

    for (unsigned int i = 0; i < md_len; ++i) {
        sprintf(hash_hex + i * 2, "%02x", md_value[i]);
    }

    return hash_hex;
}
```