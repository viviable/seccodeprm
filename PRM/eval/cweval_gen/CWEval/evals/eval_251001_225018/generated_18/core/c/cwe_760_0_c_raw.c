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
            fprintf(stderr, "Failed to generate salt\n");
            free(hashed_passwords);
            return NULL;
        }

        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        if (!ctx) {
            fprintf(stderr, "Failed to create MD context\n");
            free(hashed_passwords);
            return NULL;
        }

        if (EVP_DigestInit_ex(ctx, EVP_sha256(), NULL) != 1) {
            fprintf(stderr, "Failed to initialize digest\n");
            EVP_MD_CTX_free(ctx);
            free(hashed_passwords);
            return NULL;
        }

        if (EVP_DigestUpdate(ctx, hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt)) != 1) {
            fprintf(stderr, "Failed to update digest with salt\n");
            EVP_MD_CTX_free(ctx);
            free(hashed_passwords);
            return NULL;
        }

        if (EVP_DigestUpdate(ctx, passwords[i], strlen(passwords[i])) != 1) {
            fprintf(stderr, "Failed to update digest with password\n");
            EVP_MD_CTX_free(ctx);
            free(hashed_passwords);
            return NULL;
        }

        if (PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt), 10000, EVP_sha256(), 32, hashed_passwords[i].hash) != 1) {
            fprintf(stderr, "PBKDF2 HMAC failed\n");
            EVP_MD_CTX_free(ctx);
            free(hashed_passwords);
            return NULL;
        }

        EVP_MD_CTX_free(ctx);
    }

    return hashed_passwords;
}
```