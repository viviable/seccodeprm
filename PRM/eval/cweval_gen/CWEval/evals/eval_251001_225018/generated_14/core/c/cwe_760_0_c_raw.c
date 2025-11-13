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
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; i++) {
        // Generate a random salt
        if (!RAND_bytes(hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt))) {
            fprintf(stderr, "Failed to generate salt\n");
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }

        // Use PKCS5_PBKDF2_HMAC to hash the password
        if (!PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt,
                               sizeof(hashed_passwords[i].salt), 10000, EVP_sha256(), 32, hashed_passwords[i].hash)) {
            fprintf(stderr, "PBKDF2 HMAC failed\n");
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }
    }

    return hashed_passwords;
}
```