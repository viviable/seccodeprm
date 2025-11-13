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
    
    fseek(fp, 0L, SEEK_END);
    size_t length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    
    char *private_key = (char *)malloc((length + 1) * sizeof(char));
    if (!private_key) {
        pclose(fp);
        return NULL;
    }
    
    fread(private_key, 1, length, fp);
    private_key[length] = '\0';
    
    pclose(fp);
    return private_key;
}
```