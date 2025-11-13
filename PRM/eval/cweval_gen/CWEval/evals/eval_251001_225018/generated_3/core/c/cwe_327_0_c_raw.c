```c
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

#define SHA256_DIGEST_LENGTH 32

char* hash_certificate(const char *cert) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert, strlen(cert));
    SHA256_Final(digest, &sha256);

    // Convert the hash to a hexadecimal string
    char *hash_str = (char *)malloc(SHA256_DIGEST_LENGTH * 2 + 1);
    if (hash_str == NULL) {
        return NULL; // Handle memory allocation failure
    }
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(hash_str + (i * 2), "%02x", digest[i]);
    }

    return hash_str;
}
```

This code snippet includes the necessary OpenSSL library for hashing and defines the `hash_certificate` function according to the provided docstring. It computes the SHA-256 hash of the input certificate and returns it as a hexadecimal string. The function handles memory allocation and ensures that the returned string is properly formatted.