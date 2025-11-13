```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[256];
    char *output = NULL;
    FILE *fp;

    // Construct the openssl command
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);

    // Open a pipe to execute the command
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to run command");
        return NULL;
    }

    // Read the output from the command
    output = (char *)malloc(1);
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&output, &len, fp)) != -1) {
        output[len] = '\0'; // Ensure null-termination
    }

    // Close the pipe
    pclose(fp);

    return output;
}
```