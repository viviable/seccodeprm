```c
#include <stdio.h>
#include <stdlib.h>

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

    // Calculate the size of the output buffer based on the key length
    size_t buffer_size = key_length * 4 + 100; // Approximate size for RSA key in PEM format
    char *buffer = (char *)malloc(buffer_size);
    if (!buffer) {
        pclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, buffer_size - 1, fp);
    buffer[bytes_read] = '\0';

    pclose(fp);

    return buffer;
}
```