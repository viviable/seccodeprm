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
    
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        return NULL;
    }
    
    // Determine the size of the output buffer
    fseek(pipe, 0, SEEK_END);
    long size = ftell(pipe);
    fseek(pipe, 0, SEEK_SET);
    
    char *buffer = (char *)malloc(size + 1);
    if (!buffer) {
        pclose(pipe);
        return NULL;
    }
    
    fread(buffer, 1, size, pipe);
    buffer[size] = '\0';
    
    pclose(pipe);
    return buffer;
}
```