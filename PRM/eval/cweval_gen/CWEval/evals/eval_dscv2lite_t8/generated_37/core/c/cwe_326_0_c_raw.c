```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to execute a shell command and capture its output
char* execute_command(const char* command) {
    // Calculate the size of the buffer needed for the output
    FILE* pipe = popen(command, "r");
    if (!pipe) return NULL;

    // Allocate memory for the output buffer
    char buffer[1024];
    size_t buffer_size = sizeof(buffer);
    char* output = (char*)malloc(buffer_size);
    if (!output) {
        pclose(pipe);
        return NULL;
    }

    // Read the output from the pipe into the buffer
    size_t total_read = 0;
    while (fgets(buffer, buffer_size, pipe)) {
        size_t len = strlen(buffer);
        if (total_read + len >= buffer_size) {
            // Reallocate the buffer if it's not large enough
            buffer_size *= 2;
            char* new_output = (char*)realloc(output, buffer_size);
            if (!new_output) {
                free(output);
                pclose(pipe);
                return NULL;
            }
            output = new_output;
        }
        memcpy(output + total_read, buffer, len);
        total_read += len;
    }

    // Null-terminate the output string
    output[total_read] = '\0';

    // Close the pipe and return the output
    pclose(pipe);
    return output;
}

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
char *create_private_key(int key_length) {
    char command[256];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);

    return execute_command(command);
}
```