```c
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

#define HASH_SIZE SHA256_DIGEST_LENGTH

char* hash_certificate(const char *cert) {
    unsigned char hash[HASH_SIZE];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert, strlen(cert));
    SHA256_Final(hash, &sha256);

    // Convert the hash to a hexadecimal string
    static char hex_hash[2 * HASH_SIZE + 1];
    for(int i = 0; i < HASH_SIZE; ++i) {
        sprintf(hex_hash + (i * 2), "%02x", hash[i]);
    }

    return hex_hash;
}
```
This code snippet includes the necessary OpenSSL library for hashing and defines the `hash_certificate` function that takes a certificate as input and returns its SHA-256 hash as a hexadecimal string. The function uses SHA256_Init, SHA256_Update, and SHA256_Final from the OpenSSL library to compute the hash. The resulting hash is then converted to a hexadecimal string using a loop and the `sprintf` function. The function returns this string. Note that the returned string is stored in a static buffer, so it will be overwritten on subsequent calls to `hash_certificate`. If thread safety is required, additional measures must be taken to handle concurrent access to the static buffer.