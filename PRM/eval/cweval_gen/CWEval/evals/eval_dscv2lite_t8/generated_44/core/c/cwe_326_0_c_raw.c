```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[256];
    char *private_key;
    FILE *fp;

    // Construct the command to generate the RSA private key
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);

    // Execute the command and capture the output
    fp = popen(command, "r");
    if (!fp) {
        return NULL; // Error executing command
    }

    // Calculate the size of the output
    fseek(fp, 0, SEEK_END);
    long length = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Allocate memory for the private key
    private_key = (char *)malloc(length + 1);
    if (!private_key) {
        pclose(fp);
        return NULL; // Memory allocation failed
    }

    // Read the output into the buffer
    fread(private_key, 1, length, fp);
    private_key[length] = '\0';

    // Close the file pointer
    pclose(fp);

    return private_key;
}
```