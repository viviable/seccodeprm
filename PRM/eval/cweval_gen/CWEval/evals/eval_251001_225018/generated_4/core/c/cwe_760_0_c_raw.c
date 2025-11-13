```c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

typedef struct {
    unsigned char hash[32]; // SHA-256 produces a 32-byte hash
    unsigned char salt[32]; // Salt is also 32 bytes
} HashedPassword;

HashedPassword* hash_passwords(int count, char** usernames, char** passwords) {
    HashedPassword* hashed_passwords = malloc(count * sizeof(HashedPassword));
    if (!hashed_passwords) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    const EVP_MD* md = EVP_sha256();
    int md_len = EVP_MD_size(md);

    for (int i = 0; i < count; i++) {
        if (!RAND_bytes(hashed_passwords[i].salt, md_len)) {
            fprintf(stderr, "Failed to generate salt\n");
            free(hashed_passwords);
            return NULL;
        }

        if (!PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt, md_len, 10000, md, hashed_passwords[i].hash, md_len)) {
            fprintf(stderr, "PBKDF2 HMAC failed\n");
            free(hashed_passwords);
            return NULL;
        }
    }

    return hashed_passwords;
}
```