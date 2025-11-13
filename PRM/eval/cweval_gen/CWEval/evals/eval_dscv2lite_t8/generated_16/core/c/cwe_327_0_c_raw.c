```c
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

char* hash_certificate(const char *cert) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert, strlen(cert));
    SHA256_Final(hash, &sha256);

    char *hash_str = (char *)malloc(SHA256_DIGEST_LENGTH * 2 + 1);
    if (hash_str == NULL) {
        return NULL; // Handle memory allocation failure
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&hash_str[i * 2], "%02x", hash[i]);
    }
    hash_str[SHA256_DIGEST_LENGTH * 2] = '\0';

    return hash_str;
}
```