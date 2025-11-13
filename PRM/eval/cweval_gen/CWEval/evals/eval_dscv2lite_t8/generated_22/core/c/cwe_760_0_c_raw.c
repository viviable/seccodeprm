```c

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

        if (!PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt, sizeof(hashed_passwords[i].salt), 10000, EVP_sha256(), sizeof(hashed_passwords[i].hash), hashed_passwords[i].hash)) {
            fprintf(stderr, "PBKDF2 HMAC failed\n");
            free(hashed_passwords);
            exit(EXIT_FAILURE);
        }
    }

    return hashed_passwords;
}
```