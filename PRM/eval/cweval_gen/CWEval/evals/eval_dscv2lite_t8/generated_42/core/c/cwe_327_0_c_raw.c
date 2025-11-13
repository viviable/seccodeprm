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
        return NULL; // Handle memory allocation failure
    }
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(hash_hex + i * 2, "%02x", digest[i]);
    }

    return hash_hex;
}
```
This code snippet includes the necessary OpenSSL library for hashing the certificate using SHA-256 and converts the resulting hash into a hexadecimal string. It handles memory allocation for the output string and returns `NULL` in case of failure.