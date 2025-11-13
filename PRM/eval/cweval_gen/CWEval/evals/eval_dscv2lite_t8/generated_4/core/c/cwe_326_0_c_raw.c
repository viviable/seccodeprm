```c
#include <stdio.h>
#include <stdlib.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[256];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);
    
    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }
    
    // Determine the size of the output
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char *buffer = malloc(size + 1);
    if (!buffer) {
        pclose(fp);
        return NULL;
    }
    
    fread(buffer, 1, size, fp);
    buffer[size] = '\0';
    
    pclose(fp);
    return buffer;
}
```