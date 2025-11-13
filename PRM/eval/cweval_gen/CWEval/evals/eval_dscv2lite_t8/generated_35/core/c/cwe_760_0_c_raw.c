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

    for (int i = 0; i < count; i++) {
        if (RAND_bytes(hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt)) != 1) {
            fprintf(stderr, "Failed to generate random salt\n");
            free(hashed_passwords);
            return NULL;
        }

        PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt), 10000, EVP_sha256(), sizeof(hashed_passwords[i].hash), hashed_passwords[i].hash);
        if (!PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt), 10000, EVP_sha256(), sizeof(hashed_passwords[i].hash), hashed_passwords[i].hash)) {
            fprintf(stderr, "PBKDF2 HMAC failed\n");
            free(hashed_passwords);
            return NULL;
        }
    }

    return hashed_passwords;
}
```