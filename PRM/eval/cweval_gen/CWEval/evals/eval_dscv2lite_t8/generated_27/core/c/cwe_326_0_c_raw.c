```c
#include <stdio.h>
#include <stdlib.h>

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
    
    // Calculate the maximum possible size of the key in PEM format
    // This is a rough estimate and may need adjustment based on actual key length
    size_t buffer_size = 1024 + KEY_LENGTH / 8; // Approximate PEM overhead
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        pclose(pipe);
        return NULL;
    }
    
    size_t bytes_read = fread(buffer, 1, buffer_size - 1, pipe);
    if (bytes_read == 0 && ferror(pipe)) {
        free(buffer);
        pclose(pipe);
        return NULL;
    }
    
    buffer[bytes_read] = '\0';
    pclose(pipe);
    
    return buffer;
}
```