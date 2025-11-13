```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[100];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);
    
    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }
    
    // Determine the size of the output
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    // Allocate memory for the output
    char *output = malloc(size + 1);
    if (!output) {
        pclose(fp);
        return NULL;
    }
    
    // Read the output into the buffer
    fread(output, 1, size, fp);
    output[size] = '\0';
    
    pclose(fp);
    return output;
}
```