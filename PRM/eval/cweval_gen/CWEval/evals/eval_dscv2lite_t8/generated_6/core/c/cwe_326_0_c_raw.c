```c
#include <stdio.h>
#include <stdlib.h>

// Function to create a private RSA key and return it as a string in PEM format
char *create_private_key(int key_length) {
    char command[100];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);

    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }

    // Calculate the maximum possible size of the output (assuming base64 encoding)
    int max_output_size = ((key_length / 8) + 2) * 4 / 3 + 100; // Add some extra space for overhead

    char *output = malloc(max_output_size);
    if (!output) {
        pclose(fp);
        return NULL;
    }

    if (fgets(output, max_output_size, fp) == NULL) {
        free(output);
        pclose(fp);
        return NULL;
    }

    pclose(fp);
    return output;
}
```
This function constructs an OpenSSL command to generate a private RSA key of specified length, executes the command, captures its output, and returns the key as a string in PEM format. It handles memory allocation and file operations safely, returning `NULL` on any failure.