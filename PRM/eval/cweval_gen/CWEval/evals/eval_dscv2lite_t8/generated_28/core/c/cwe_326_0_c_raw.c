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
    
    // Determine the size of the output
    fseek(fp, 0L, SEEK_END);
    long length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    
    // Allocate memory for the output
    char *buffer = (char *)malloc(length + 1);
    if (!buffer) {
        pclose(fp);
        return NULL;
    }
    
    // Read the output from the pipe into the buffer
    fread(buffer, 1, length, fp);
    buffer[length] = '\0';
    
    pclose(fp);
    return buffer;
}
```