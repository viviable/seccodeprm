```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[100];
    char buffer[4096];
    FILE *fp;

    // Construct the command to generate the RSA private key
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);

    // Open a pipe to execute the command
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to run command");
        return NULL;
    }

    // Read the output from the command
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        pclose(fp);
        return NULL;
    }

    // Close the pipe
    pclose(fp);

    // Duplicate the buffer to return a copy of the key
    char *key = strdup(buffer);
    if (key == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }

    return key;
}
```