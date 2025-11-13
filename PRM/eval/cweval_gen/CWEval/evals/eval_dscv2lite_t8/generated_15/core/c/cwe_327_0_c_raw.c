```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define HEX_STRING_LENGTH SHA256_DIGEST_LENGTH * 2 + 1

char* hash_certificate(const char *cert) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert, strlen(cert));
    SHA256_Final(hash, &sha256);

    char *hex_string = (char *)malloc(HEX_STRING_LENGTH);
    if (hex_string == NULL) {
        return NULL; // Handle memory allocation failure
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hex_string + i * 2, "%02x", hash[i]);
    }
    hex_string[HEX_STRING_LENGTH - 1] = '\0';

    return hex_string;
}
```
This code uses the OpenSSL library to compute the SHA-256 hash of a given certificate and returns it as a hexadecimal string. The `hash_certificate` function initializes an SHA-256 context, updates it with the certificate data, finalizes the hash computation, and then formats the resulting hash into a hexadecimal string. The function dynamically allocates memory for the returned string, so it is the caller's responsibility to free this memory when it is no longer needed. If memory allocation fails, the function returns `NULL`.