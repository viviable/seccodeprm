```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

char* hash_certificate(const char *cert) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, cert, strlen(cert));
    SHA256_Final(hash, &sha256);

    // Convert the hash to a hexadecimal string
    static char hex_string[2 * SHA256_DIGEST_LENGTH + 1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hex_string + (i * 2), "%02x", hash[i]);
    }
    hex_string[2 * SHA256_DIGEST_LENGTH] = '\0';

    return hex_string;
}
```

This solution includes the necessary OpenSSL headers and implements the `hash_certificate` function that securely hashes a given certificate using SHA-256 and returns the result as a hexadecimal string. The function uses a static buffer to store the resulting hexadecimal string, which is then returned. Note that this approach assumes that the input certificate is a null-terminated string and does not handle memory allocation errors from OpenSSL functions like `SHA256_Init`, `SHA256_Update`, and `SHA256_Final`. In a production environment, you would need to add appropriate error checking and memory management.