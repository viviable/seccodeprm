```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[128];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);
    
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        return NULL;
    }
    
    // Determine the size of the output
    fseek(pipe, 0, SEEK_END);
    long size = ftell(pipe);
    fseek(pipe, 0, SEEK_SET);
    
    // Allocate memory for the output
    char *output = (char *)malloc(size + 1);
    if (!output) {
        pclose(pipe);
        return NULL;
    }
    
    // Read the output from the pipe
    fread(output, 1, size, pipe);
    output[size] = '\0';
    
    pclose(pipe);
    return output;
}
```