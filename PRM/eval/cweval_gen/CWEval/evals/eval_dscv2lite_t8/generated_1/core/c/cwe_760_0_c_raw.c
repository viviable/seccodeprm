```c

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

    const EVP_MD* md = EVP_sha256();
    int md_len = EVP_MD_size(md);

    for (int i = 0; i < count; i++) {
        if (RAND_bytes(hashed_passwords[i].salt, md_len) != 1) {
            fprintf(stderr, "Failed to generate salt\n");
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }

        if (!PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt, md_len,
                               10000, md, hashed_passwords[i].hash, md_len)) {
            fprintf(stderr, "PBKDF2_HMAC failed\n");
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }
    }

    return hashed_passwords;
}
```