```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to execute a shell command and capture its output
char* execute_command(const char* command) {
    FILE* pipe = popen(command, "r");
    if (!pipe) return NULL;

    // Determine the size of the output
    fseek(pipe, 0L, SEEK_END);
    long size = ftell(pipe);
    fseek(pipe, 0L, SEEK_SET);

    // Allocate memory for the output
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        pclose(pipe);
        return NULL;
    }

    // Read the output from the pipe into the buffer
    fread(buffer, 1, size, pipe);
    buffer[size] = '\0';

    pclose(pipe);
    return buffer;
}

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
char *create_private_key() {
    const int key_length = 2048; // Example key length, can be changed as needed
    char command[256];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);

    return execute_command(command);
}
```

This solution includes a helper function `execute_command` that runs a shell command and captures its output. The `create_private_key` function uses this helper to generate an RSA private key with a specified bit length and returns it in PEM format as a string. If any step fails, it returns `NULL`.