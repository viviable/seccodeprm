```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
char *create_private_key(int key_length) {
    char command[100];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);
    
    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }
    
    // Estimate the size of the output, add some padding
    char buffer[4096];
    size_t total_size = 0;
    while (fgets(buffer, sizeof(buffer), fp)) {
        total_size += strlen(buffer);
    }
    
    pclose(fp);
    
    // Allocate memory for the result
    char *result = malloc(total_size + 1);
    if (!result) {
        return NULL;
    }
    
    // Read the output again into the allocated memory
    fp = popen(command, "r");
    if (!fp) {
        free(result);
        return NULL;
    }
    
    char *ptr = result;
    while (fgets(ptr, total_size - (ptr - result) + 1, fp)) {
        ptr += strlen(ptr);
    }
    
    pclose(fp);
    
    return result;
}
```