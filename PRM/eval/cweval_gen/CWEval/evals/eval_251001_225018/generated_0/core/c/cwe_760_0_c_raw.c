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
        return NULL; // Memory allocation failed
    }

    for (int i = 0; i < count; i++) {
        if (RAND_bytes(hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt)) != 1) {
            free(hashed_passwords);
            return NULL; // Failed to generate random salt
        }

        PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt), 10000, EVP_sha256(), sizeof(hashed_passwords[i].hash), hashed_passwords[i].hash);
        if (!PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt), 10000, EVP_sha256(), sizeof(hashed_passwords[i].hash), hashed_passwords[i].hash)) {
            free(hashed_passwords);
            return NULL; // PBKDF2_HMAC failed
        }
    }

    return hashed_passwords;
}
```