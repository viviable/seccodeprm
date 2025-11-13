```c

    for (int i = 0; i < count; i++) {
        if (!RAND_bytes(hashed_passwords[i].salt, 32)) {
            fprintf(stderr, "Failed to generate salt\n");
            free(hashed_passwords);
            return NULL;
        }

        if (!PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt, 32, 10000, EVP_sha256(), hashed_passwords[i].hash, 32)) {
            fprintf(stderr, "Failed to hash password\n");
            free(hashed_passwords);
            return NULL;
        }
    }

    return hashed_passwords;
}
```