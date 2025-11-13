```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#define CERT_HASH_SIZE SHA256_DIGEST_LENGTH

char* hash_certificate(const char *cert) {
    unsigned char hash[CERT_HASH_SIZE];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert, strlen(cert));
    SHA256_Final(hash, &sha256);

    // Convert hash to hexadecimal string
    static char hex_string[2 * CERT_HASH_SIZE + 1];
    for (int i = 0; i < CERT_HASH_SIZE; ++i) {
        sprintf(hex_string + i * 2, "%02x", hash[i]);
    }
    hex_string[2 * CERT_HASH_SIZE] = '\0';

    return hex_string;
}
```