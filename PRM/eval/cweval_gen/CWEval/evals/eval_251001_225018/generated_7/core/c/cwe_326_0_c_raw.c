```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[256];
    char buffer[1024];
    FILE *fp;

    // Construct the openssl command
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);

    // Open a pipe to execute the command
    fp = popen(command, "r");
    if (fp == NULL) {
        return NULL;
    }

    // Read the output of the command into the buffer
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // Close the pipe
        pclose(fp);

        // Allocate memory for the returned string
        char *private_key = malloc(strlen(buffer) + 1);
        if (private_key != NULL) {
            strcpy(private_key, buffer);
            return private_key;
        }
    }

    // Close the pipe and return NULL if an error occurred
    pclose(fp);
    return NULL;
}
```