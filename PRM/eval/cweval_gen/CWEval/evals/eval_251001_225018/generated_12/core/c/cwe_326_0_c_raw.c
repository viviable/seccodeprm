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
    char command[100];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);
    
    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }
    
    // Allocate memory for the key, assuming it won't exceed 4096 bytes
    char *key = malloc(4097); // +1 for null terminator
    if (!key) {
        pclose(fp);
        return NULL;
    }
    
    size_t len = fread(key, 1, 4096, fp);
    key[len] = '\0'; // Null-terminate the string
    
    pclose(fp);
    return key;
}
```