```c
#include <stdio.h>
#include <stdlib.h>

#define KEY_LENGTH 2048

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
char *create_private_key() {
    char command[100];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);
    
    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }
    
    // Calculate the maximum size of the key based on the key length
    int max_size = ((KEY_LENGTH + 7) / 8) * 4 + 64; // Base64 encoding and some extra space
    
    char *key_buffer = (char *)malloc(max_size);
    if (!key_buffer) {
        pclose(fp);
        return NULL;
    }
    
    if (fgets(key_buffer, max_size, fp)) {
        // Remove trailing newline if present
        size_t len = strlen(key_buffer);
        if (len > 0 && key_buffer[len - 1] == '\n') {
            key_buffer[len - 1] = '\0';
        }
        pclose(fp);
        return key_buffer;
    } else {
        free(key_buffer);
        pclose(fp);
        return NULL;
    }
}
```