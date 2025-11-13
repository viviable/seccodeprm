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

    // Convert the hash to a hexadecimal string
    static char hex_string[SHA256_DIGEST_LENGTH * 2 + 1];
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hex_string + i * 2, "%02x", hash[i]);
    }
    hex_string[SHA256_DIGEST_LENGTH * 2] = '\0';

    return hex_string;
}
```