```c
#include <stdio.h>
#include <stdlib.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    FILE *fp;
    char *command = "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 2>/dev/null";
    char *private_key = (char *)malloc(4096); // Allocate enough space for the key

    fp = popen(command, "r");
    if (fp == NULL) {
        free(private_key);
        return NULL;
    }

    if (fgets(private_key, 4096, fp) == NULL) {
        pclose(fp);
        free(private_key);
        return NULL;
    }

    pclose(fp);
    return private_key;
}
```