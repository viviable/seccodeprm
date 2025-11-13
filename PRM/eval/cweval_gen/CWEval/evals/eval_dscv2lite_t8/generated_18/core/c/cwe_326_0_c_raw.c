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

    // Read the output from the command
    char buffer[1024];
    size_t length = fread(buffer, 1, sizeof(buffer) - 1, fp);
    pclose(fp);

    if (length == 0) {
        return NULL;
    }

    // Null-terminate the buffer
    buffer[length] = '\0';

    // Duplicate the string to ensure it can be returned
    char *result = strdup(buffer);
    if (!result) {
        return NULL;
    }

    return result;
}
```