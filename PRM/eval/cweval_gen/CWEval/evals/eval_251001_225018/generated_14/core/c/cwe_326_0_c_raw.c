```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[100];
    char *output;
    FILE *fp;

    // Construct the command to generate the RSA private key
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);

    // Open a pipe to execute the command
    fp = popen(command, "r");
    if (fp == NULL) {
        return NULL;
    }

    // Allocate memory to store the output of the command
    output = malloc(4096);
    if (output == NULL) {
        pclose(fp);
        return NULL;
    }

    // Read the output from the command into the allocated memory
    if (fgets(output, 4096, fp) == NULL) {
        free(output);
        pclose(fp);
        return NULL;
    }

    // Close the pipe
    pclose(fp);

    return output;
}
```