```c
#include <stdio.h>
#include <stdlib.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[100];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);
    
    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }
    
    // Determine the size of the output
    fseek(fp, 0L, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    
    // Allocate memory for the key
    char *key = malloc(sz + 1);
    if (!key) {
        pclose(fp);
        return NULL;
    }
    
    // Read the key from the pipe
    fread(key, 1, sz, fp);
    key[sz] = '\0';
    
    pclose(fp);
    return key;
}
```