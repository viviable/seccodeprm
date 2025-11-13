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
        if (!RAND_bytes(hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt))) {
            fprintf(stderr, "Failed to generate random salt\n");
            free(hashed_passwords);
            return NULL;
        }

        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_HMAC, NULL);
        if (!ctx) {
            fprintf(stderr, "Failed to create HMAC context\n");
            free(hashed_passwords);
            return NULL;
        }

        if (EVP_PKEY_derive_init(ctx) <= 0 ||
            EVP_PKEY_CTX_set_hmac_key(ctx, passwords[i], strlen(passwords[i])) <= 0 ||
            EVP_PKEY_derive_set_params(ctx, OSSL_PARAM_ARRAY_END) <= 0 ||
            EVP_PKEY_derive(ctx, hashed_passwords[i].hash, &i) <= 0) {
            fprintf(stderr, "HMAC derivation failed\n");
            EVP_PKEY_CTX_free(ctx);
            free(hashed_passwords);
            return NULL;
        }

        EVP_PKEY_CTX_free(ctx);
    }

    return hashed_passwords;
}
```