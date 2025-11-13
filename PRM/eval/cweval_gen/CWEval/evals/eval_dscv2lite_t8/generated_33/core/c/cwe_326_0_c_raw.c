```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
char *create_private_key() {
    char command[256];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);

    FILE *pipe = popen(command, "r");
    if (!pipe) {
        return NULL;
    }

    // Calculate the maximum size of the key output
    // Assuming a worst-case scenario where each byte is represented by 4 characters (including '\n' and '=')
    size_t max_size = (KEY_LENGTH / 8) * 4 + 100; // Adding extra space for overhead
    char *buffer = (char *)malloc(max_size);
    if (!buffer) {
        pclose(pipe);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, max_size - 1, pipe);
    buffer[bytes_read] = '\0';

    int ret = pclose(pipe);
    if (ret != 0) {
        free(buffer);
        return NULL;
    }

    return buffer;
}
```