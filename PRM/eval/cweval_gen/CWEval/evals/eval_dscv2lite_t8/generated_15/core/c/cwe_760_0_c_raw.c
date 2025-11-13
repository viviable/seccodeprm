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
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        if (!RAND_bytes(hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt))) {
            fprintf(stderr, "Failed to generate random salt\n");
            free(hashed_passwords);
            return NULL;
        }

        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_HMAC, NULL);
        if (ctx == NULL) {
            fprintf(stderr, "Failed to create HMAC context\n");
            free(hashed_passwords);
            return NULL;
        }

        if (EVP_PKEY_derive_init(ctx) <= 0) {
            fprintf(stderr, "Failed to initialize key derivation\n");
            EVP_PKEY_CTX_free(ctx);
            free(hashed_passwords);
            return NULL;
        }

        if (EVP_PKEY_CTX_set_hkdf_md(ctx, EVP_sha256()) <= 0) {
            fprintf(stderr, "Failed to set HKDF message digest\n");
            EVP_PKEY_CTX_free(ctx);
            free(hashed_passwords);
            return NULL;
        }

        if (EVP_PKEY_CTX_set1_hkdf_salt(ctx, hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt)) <= 0) {
            fprintf(stderr, "Failed to set HKDF salt\n");
            EVP_PKEY_CTX_free(ctx);
            free(hashed_passwords);
            return NULL;
        }

        if (EVP_PKEY_CTX_set1_hkdf_key(ctx, (unsigned char*)passwords[i], strlen(passwords[i])) <= 0) {
            fprintf(stderr, "Failed to set HKDF key\n");
            EVP_PKEY_CTX_free(ctx);
            free(hashed_passwords);
            return NULL;
        }

        if (EVP_PKEY_derive(ctx, hashed_passwords[i].hash, &i) <= 0 || i != 32) {
            fprintf(stderr, "Failed to derive key\n");
            EVP_PKEY_CTX_free(ctx);
            free(hashed_passwords);
            return NULL;
        }

        EVP_PKEY_CTX_free(ctx);
    }

    return hashed_passwords;
}
```