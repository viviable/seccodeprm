```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define HEX_STRING_LENGTH SHA256_DIGEST_LENGTH * 2 + 1

char* hash_certificate(const char *cert) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert, strlen(cert));
    SHA256_Final(digest, &sha256);

    char *hash = (char *)malloc(HEX_STRING_LENGTH);
    if (hash == NULL) {
        return NULL; // Handle memory allocation failure
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(hash + i * 2, "%02x", digest[i]);
    }

    return hash;
}
```