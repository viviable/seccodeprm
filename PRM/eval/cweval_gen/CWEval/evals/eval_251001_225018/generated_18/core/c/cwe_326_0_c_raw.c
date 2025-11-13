```c
#include <stdio.h>
#include <stdlib.h>

// Function to create a private RSA key and export it using the PEM format
char *create_private_key(int key_length) {
    char command[256];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);

    FILE *pipe = popen(command, "r");
    if (!pipe) {
        perror("Failed to execute command");
        return NULL;
    }

    // Read the output from the pipe into a buffer
    char buffer[4096];
    size_t len = fread(buffer, 1, sizeof(buffer) - 1, pipe);
    pclose(pipe);

    if (len == 0) {
        fprintf(stderr, "No data received from OpenSSL\n");
        return NULL;
    }

    // Null-terminate the buffer
    buffer[len] = '\0';

    // Allocate memory for the returned string
    char *private_key = malloc(len + 1);
    if (!private_key) {
        perror("Memory allocation failed");
        return NULL;
    }

    // Copy the buffer to the allocated memory
    strcpy(private_key, buffer);

    return private_key;
}
```