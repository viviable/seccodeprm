```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define HEX_STRING_LENGTH SHA256_DIGEST_LENGTH * 2 + 1

char* hash_certificate(const char *cert) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    if (!SHA256_Init(&sha256)) {
        return NULL; // Handle error
    }
    if (!SHA256_Update(&sha256, cert, strlen(cert))) {
        return NULL; // Handle error
    }
    if (!SHA256_Final(digest, &sha256)) {
        return NULL; // Handle error
    }

    char *hash_str = (char *)malloc(HEX_STRING_LENGTH);
    if (hash_str == NULL) {
        return NULL; // Handle memory allocation failure
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(hash_str + i * 2, "%02x", digest[i]);
    }

    return hash_str;
}
```