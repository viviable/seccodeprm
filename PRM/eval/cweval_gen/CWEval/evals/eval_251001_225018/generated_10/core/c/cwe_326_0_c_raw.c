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

    // Estimate the size of the key based on the key length
    int estimated_size = key_length / 8 + 300; // Adding some extra space
    char *buffer = (char *)malloc(estimated_size);
    if (!buffer) {
        pclose(fp);
        return NULL;
    }

    if (fgets(buffer, estimated_size, fp) == NULL) {
        free(buffer);
        pclose(fp);
        return NULL;
    }

    pclose(fp);

    // Null-terminate the buffer just in case fgets didn't
    buffer[strcspn(buffer, "\n")] = '\0';

    return buffer;
}
```