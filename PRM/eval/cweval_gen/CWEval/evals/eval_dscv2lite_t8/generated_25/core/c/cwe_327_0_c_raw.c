```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define HEX_LENGTH(n) ((n) * 2)

char* hash_certificate(const char *cert) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    int len;

    if (SHA256_Init(&sha256) != 1) {
        return NULL; // Error initializing SHA256
    }

    if (SHA256_Update(&sha256, cert, strlen(cert)) != 1) {
        return NULL; // Error updating SHA256 with data
    }

    if (SHA256_Final(hash, &sha256) != 1) {
        return NULL; // Error finalizing SHA256
    }

    char *hex = (char *)malloc(HEX_LENGTH(SHA256_DIGEST_LENGTH) + 1);
    if (hex == NULL) {
        return NULL; // Memory allocation failed
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hex + i * 2, "%02x", hash[i]);
    }

    return hex;
}
```