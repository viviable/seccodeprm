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

    // Convert hash to hexadecimal string
    char *hex_str = malloc(SHA256_DIGEST_LENGTH * 2 + 1);
    for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        sprintf(hex_str + i * 2, "%02x", hash[i]);
    }

    return hex_str;
}
```

Note: This solution uses the OpenSSL library for hashing. Make sure to link against the OpenSSL library when compiling this code. For example, if you are using gcc, you would compile it with `gcc -o hash_cert hash_cert.c -lssl -lcrypto`.