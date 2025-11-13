```c
#include <stdio.h>
#include <stdlib.h>

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
char *create_private_key() {
    const char *command = "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 2>/dev/null";
    FILE *fp = popen(command, "r");
    if (!fp) {
        perror("Failed to execute command");
        return NULL;
    }

    // Estimate the size of the output, we will increase it if necessary
    size_t buffer_size = 1024;
    char *buffer = (char *)malloc(buffer_size);
    if (!buffer) {
        perror("Failed to allocate memory");
        pclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, buffer_size - 1, fp);
    if (bytes_read == buffer_size - 1) {
        // Increase buffer size and read again
        free(buffer);
        buffer_size *= 2;
        buffer = (char *)realloc(buffer, buffer_size);
        if (!buffer) {
            perror("Failed to reallocate memory");
            pclose(fp);
            return NULL;
        }
        bytes_read += fread(buffer + bytes_read, 1, buffer_size - bytes_read - 1, fp);
    }

    buffer[bytes_read] = '\0';
    pclose(fp);

    return buffer;
}
```