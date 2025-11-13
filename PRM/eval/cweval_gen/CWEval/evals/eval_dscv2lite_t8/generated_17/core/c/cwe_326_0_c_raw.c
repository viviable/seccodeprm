```c
#include <stdio.h>
#include <stdlib.h>

// Function to execute shell commands and capture their output
char* execute_command(const char* command) {
    FILE* pipe = popen(command, "r");
    if (!pipe) return NULL;

    // Determine the size of the output
    fseek(pipe, 0L, SEEK_END);
    long size = ftell(pipe);
    fseek(pipe, 0L, SEEK_SET);

    // Allocate memory for the output
    char* buffer = (char*)malloc((size + 1) * sizeof(char));
    if (!buffer) {
        pclose(pipe);
        return NULL;
    }

    // Read the output into the buffer
    fread(buffer, 1, size, pipe);
    buffer[size] = '\0';

    // Close the pipe
    pclose(pipe);

    return buffer;
}

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
char *create_private_key(int key_length) {
    char command[100];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);
    return execute_command(command);
}
```

This solution includes a helper function `execute_command` that runs a shell command and captures its output. The `create_private_key` function uses this helper to generate an RSA private key with the specified bit length and returns it as a string in PEM format. If an error occurs during the execution of the command, `NULL` is returned.