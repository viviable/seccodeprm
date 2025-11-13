```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
char *create_private_key(int key_length) {
    char command[100];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);

    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }

    // Estimate the size of the key (this is a rough estimate)
    size_t key_size_estimate = key_length / 8 + 100; // Adding some extra space for the header and footer
    char *key = malloc(key_size_estimate);
    if (!key) {
        pclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(key, 1, key_size_estimate, fp);
    if (bytes_read == 0 && ferror(fp)) {
        free(key);
        pclose(fp);
        return NULL;
    }

    key[bytes_read] = '\0';
    pclose(fp);

    return key;
}
```