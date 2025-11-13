```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define HASH_SIZE SHA256_DIGEST_LENGTH

char* hash_certificate(const char *cert) {
    unsigned char hash[HASH_SIZE];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert, strlen(cert));
    SHA256_Final(hash, &sha256);

    // Convert hash to hexadecimal string
    static char hex_string[2 * HASH_SIZE + 1];
    for (int i = 0; i < HASH_SIZE; ++i) {
        sprintf(hex_string + i * 2, "%02x", hash[i]);
    }
    hex_string[2 * HASH_SIZE] = '\0';

    return hex_string;
}
```

This code snippet includes the necessary OpenSSL library to perform the SHA-256 hashing of a certificate. It defines a function `hash_certificate` that takes a certificate string as input and returns its SHA-256 hash as a hexadecimal string. The function uses OpenSSL's SHA256 functions to compute the hash and then converts the resulting bytes into a human-readable hexadecimal format.