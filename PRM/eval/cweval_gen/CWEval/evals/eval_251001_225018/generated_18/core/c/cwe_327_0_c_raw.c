```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define SHA256_DIGEST_LENGTH 32

char* hash_certificate(const char *cert) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert, strlen(cert));
    SHA256_Final(digest, &sha256);

    // Convert the hash to a hexadecimal string
    char *hash_hex = malloc(SHA256_DIGEST_LENGTH * 2 + 1);
    if (hash_hex == NULL) {
        return NULL; // Memory allocation failed
    }
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(hash_hex + i * 2, "%02x", digest[i]);
    }

    return hash_hex;
}
```
This function uses the OpenSSL library to compute the SHA-256 hash of a given certificate. It then converts the resulting binary digest into a hexadecimal string representation. If memory allocation fails, it returns `NULL`.