```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to create a private RSA key and export it in PEM format
char *create_private_key(int key_length) {
    // Command to generate RSA private key
    char command[100];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);

    // Execute the command and capture the output
    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }

    // Determine the size of the output
    fseek(fp, 0L, SEEK_END);
    long length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    // Allocate memory for the output
    char *output = (char *)malloc(length + 1);
    if (!output) {
        pclose(fp);
        return NULL;
    }

    // Read the output from the pipe
    fread(output, 1, length, fp);
    fclose(fp);
    output[length] = '\0';

    return output;
}
```