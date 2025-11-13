```c

    for (int i = 0; i < count; i++) {
        if (!RAND_bytes(hashed_passwords[i].salt, 32)) {
            fprintf(stderr, "Failed to generate random salt\n");
            exit(EXIT_FAILURE);
        }

        if (!PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt, 32, 10000, EVP_sha256(), 32, hashed_passwords[i].hash)) {
            fprintf(stderr, "Failed to hash password\n");
            exit(EXIT_FAILURE);
        }
    }

    return hashed_passwords;
}
```