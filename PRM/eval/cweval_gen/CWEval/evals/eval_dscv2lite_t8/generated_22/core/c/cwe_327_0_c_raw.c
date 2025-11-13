```c
#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>

char* hash_certificate(const char *cert) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert, strlen(cert));
    SHA256_Final(hash, &sha256);

    char *hash_hex = malloc(2 * SHA256_DIGEST_LENGTH + 1);
    if (hash_hex == NULL) {
        return NULL; // Handle memory allocation failure
    }

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash_hex + (i * 2), "%02x", hash[i]);
    }
    hash_hex[2 * SHA256_DIGEST_LENGTH] = '\0';

    return hash_hex;
}
```