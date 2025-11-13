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

    for (int i = 0; i < count; i++) {
        if (!RAND_bytes(hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt))) {
            fprintf(stderr, "Failed to generate random salt\n");
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }

        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_sha256(), NULL);
        if (ctx == NULL) {
            fprintf(stderr, "Failed to create context\n");
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }

        if (EVP_PKEY_derive_init(ctx) != 1) {
            fprintf(stderr, "Failed to initialize derivation\n");
            EVP_PKEY_CTX_free(ctx);
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }

        if (EVP_PKEY_CTX_set_hkdf_md(ctx, EVP_sha256()) != 1) {
            fprintf(stderr, "Failed to set HMAC algorithm\n");
            EVP_PKEY_CTX_free(ctx);
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }

        if (EVP_PKEY_CTX_set_hkdf_salt(ctx, hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt)) != 1) {
            fprintf(stderr, "Failed to set salt\n");
            EVP_PKEY_CTX_free(ctx);
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }

        if (EVP_PKEY_CTX_set_hkdf_key(ctx, (unsigned char *)passwords[i], strlen(passwords[i])) != 1) {
            fprintf(stderr, "Failed to set key\n");
            EVP_PKEY_CTX_free(ctx);
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }

        if (EVP_PKEY_derive(ctx, hashed_passwords[i].hash, &hashed_passwords[i].hash[32]) <= 0) {
            fprintf(stderr, "Derivation failed\n");
            EVP_PKEY_CTX_free(ctx);
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }

        EVP_PKEY_CTX_free(ctx);
    }

    return hashed_passwords;
}
```