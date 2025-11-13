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
    char command[128];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);
    
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        return NULL;
    }
    
    // Calculate the approximate size of the key (PEM format can be larger)
    size_t size = KEY_LENGTH / 8 + KEY_LENGTH / 64 + 50; // Rough estimation
    char *buffer = malloc(size);
    if (!buffer) {
        pclose(pipe);
        return NULL;
    }
    
    size_t bytesRead = fread(buffer, 1, size - 1, pipe);
    buffer[bytesRead] = '\0';
    
    pclose(pipe);
    
    // Resize the buffer to fit exactly the data read
    buffer = realloc(buffer, bytesRead + 1);
    if (!buffer) {
        return NULL;
    }
    
    return buffer;
}
```