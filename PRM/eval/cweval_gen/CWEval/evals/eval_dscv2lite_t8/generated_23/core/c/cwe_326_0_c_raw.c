```c
#include <stdio.h>
#include <stdlib.h>

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
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Allocate memory for the output
    char *output = malloc(len + 1);
    if (!output) {
        pclose(fp);
        return NULL;
    }

    // Read the output into the allocated memory
    fread(output, 1, len, fp);
    output[len] = '\0';

    // Close the pipe
    pclose(fp);

    return output;
}
```