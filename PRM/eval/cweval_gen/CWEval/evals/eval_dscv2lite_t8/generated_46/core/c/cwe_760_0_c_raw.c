```c

    for (int i = 0; i < count; i++) {
        if (!RAND_bytes(hashed_passwords[i].salt, 32)) {
            fprintf(stderr, "Error generating random salt\n");
            exit(EXIT_FAILURE);
        }

        PKCS5_PBKDF2_HMAC(passwords[i], strlen(passwords[i]), hashed_passwords[i].salt, 32,
                           10000, EVP_sha256(), 32, hashed_passwords[i].hash);
    }

    return hashed_passwords;
}
```