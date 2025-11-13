```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
char *create_private_key() {
    const char *command = "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 2>/dev/null";
    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }

    // Allocate memory for the key, assuming it won't exceed 16KB
    char *key = (char *)malloc(16384);
    if (!key) {
        pclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(key, 1, 16383, fp);
    key[bytes_read] = '\0';

    pclose(fp);

    // Check if the key was successfully generated
    if (strstr(key, "-----BEGIN RSA PRIVATE KEY-----") == NULL) {
        free(key);
        return NULL;
    }

    return key;
}
```