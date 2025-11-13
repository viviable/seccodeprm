```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create a private RSA key and return it as a string in PEM format
char *create_private_key(int key_length) {
    char command[100];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);
    
    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }
    
    // Determine the size of the output
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char *buffer = (char *)malloc(len + 1);
    if (!buffer) {
        pclose(fp);
        return NULL;
    }
    
    fread(buffer, 1, len, fp);
    buffer[len] = '\0';
    
    pclose(fp);
    return buffer;
}
```