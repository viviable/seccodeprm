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

    if (hashed_passwords == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    const EVP_MD* md = EVP_sha256();
    int iterations = 10000;
    unsigned char salt[32];

    for (int i = 0; i < count; i++) {
        // Generate a random salt
        if (!RAND_bytes(salt, sizeof(salt))) {
            fprintf(stderr, "Failed to generate salt\n");
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }

        // Hash the password using PBKDF2 with HMAC-SHA256
        if (PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), salt, sizeof(salt), iterations, md, hashed_passwords[i].hash, sizeof(hashed_passwords[i].hash)) != 1) {
            fprintf(stderr, "PBKDF2_HMAC failed\n");
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }

        // Copy the salt to the structure
        memcpy(hashed_passwords[i].salt, salt, sizeof(salt));
    }

    return hashed_passwords;
}
```