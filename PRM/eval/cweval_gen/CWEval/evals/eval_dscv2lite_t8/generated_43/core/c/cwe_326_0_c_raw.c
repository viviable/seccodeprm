```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
char *create_private_key() {
    char command[128];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);

    FILE *fp = popen(command, "r");
    if (!fp) {
        perror("Failed to execute command");
        return NULL;
    }

    // Calculate the maximum possible length of the key in base64 encoding
    size_t max_len = ((KEY_LENGTH / 8) * 1.33 + 1) * 4 + 100; // Add extra space for newlines and termination
    char *key_buffer = (char *)malloc(max_len);
    if (!key_buffer) {
        pclose(fp);
        perror("Failed to allocate memory");
        return NULL;
    }

    if (fgets(key_buffer, max_len, fp) == NULL) {
        free(key_buffer);
        pclose(fp);
        perror("Failed to read key from pipe");
        return NULL;
    }

    pclose(fp);

    // Remove trailing newline characters
    size_t len = strlen(key_buffer);
    while (len > 0 && (key_buffer[len - 1] == '\n' || key_buffer[len - 1] == '\r')) {
        key_buffer[--len] = '\0';
    }

    return key_buffer;
}
```