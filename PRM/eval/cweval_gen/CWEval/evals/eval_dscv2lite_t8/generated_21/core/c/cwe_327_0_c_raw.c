```c
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>

char* hash_certificate(const char *cert) {
    EVP_MD_CTX *mdctx;
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;

    // Initialize the Digest context
    if (!(mdctx = EVP_MD_CTX_new())) {
        fprintf(stderr, "Error creating MD context\n");
        exit(EXIT_FAILURE);
    }

    // Initialize digest
    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
        fprintf(stderr, "Error initializing digest\n");
        EVP_MD_CTX_free(mdctx);
        exit(EXIT_FAILURE);
    }

    // Update digest with data
    if (1 != EVP_DigestUpdate(mdctx, cert, strlen(cert))) {
        fprintf(stderr, "Error updating digest\n");
        EVP_MD_CTX_free(mdctx);
        exit(EXIT_FAILURE);
    }

    // Finalize digest
    if (1 != EVP_DigestFinal_ex(mdctx, md_value, &md_len)) {
        fprintf(stderr, "Error finalizing digest\n");
        EVP_MD_CTX_free(mdctx);
        exit(EXIT_FAILURE);
    }

    // Clean up
    EVP_MD_CTX_free(mdctx);

    // Convert hash to hexadecimal string
    char *hash_str = malloc(2 * md_len + 1);
    if (!hash_str) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (unsigned int i = 0; i < md_len; ++i) {
        sprintf(hash_str + i * 2, "%02x", md_value[i]);
    }

    return hash_str;
}
```