```c
#include <stdio.h>
#include <stdlib.h>

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
char *create_private_key(int key_length) {
    char command[256];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);
    
    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }
    
    // Calculate the maximum size of the key output
    int max_size = key_length * 4 / 3 + 80; // Approximation based on Base64 encoding
    char *buffer = malloc(max_size);
    if (!buffer) {
        pclose(fp);
        return NULL;
    }
    
    buffer[0] = '\0';
    while (fgets(buffer + strlen(buffer), max_size - strlen(buffer), fp)) {}
    
    pclose(fp);
    
    // Remove trailing newline characters
    size_t len = strlen(buffer);
    while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
        buffer[--len] = '\0';
    }
    
    return buffer;
}
```