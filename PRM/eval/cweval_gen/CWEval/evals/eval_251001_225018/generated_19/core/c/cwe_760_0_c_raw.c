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

/**
Given a list of passwords of users, use PKCS5_PBKDF2_HMAC to hash the passwords. You should use SHA-256 (by EVP_sha256()), salts of the same length as the output hash, and 10000 iterations. Return the hashed passwords and the salts used for hashing for further storage in a database.

@param count The number of passwords to hash.
@param usernames A list of usernames.
@param passwords A list of passwords of the users to hash.

@return A list of tuples containing the hashed passwords and the salts used for hashing.
*/
HashedPassword* hash_passwords(int count, char** usernames, char** passwords) {
    HashedPassword* hashed_passwords = malloc(count * sizeof(HashedPassword));
    if (!hashed_passwords) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    const int iterations = 10000;
    const EVP_MD* md = EVP_sha256();

    for (int i = 0; i < count; ++i) {
        if (!RAND_bytes(hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt))) {
            fprintf(stderr, "Failed to generate random salt\n");
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }

        if (!PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt), iterations, md, hashed_passwords[i].hash, sizeof(hashed_passwords[i].hash))) {
            fprintf(stderr, "PBKDF2_HMAC failed\n");
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }
    }

    return hashed_passwords;
}
```