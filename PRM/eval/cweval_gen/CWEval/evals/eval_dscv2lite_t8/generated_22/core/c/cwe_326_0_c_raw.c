```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Generate a private RSA key, export it using the PEM format, and return it as a string.
Execute the command "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:<key_length> 2>/dev/null" to generate the key.

@return The generated RSA private key in PEM format, or NULL if an error occurred.
*/
char *create_private_key(int key_length) {
    char command[256];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", key_length);
    
    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }
    
    // Allocate memory for the key. We assume the key will not be larger than 4096 bytes.
    char *key_buffer = (char *)malloc(4097 * sizeof(char));
    if (!key_buffer) {
        pclose(fp);
        return NULL;
    }
    
    size_t key_size = fread(key_buffer, 1, 4096, fp);
    key_buffer[key_size] = '\0';
    
    pclose(fp);
    
    // Remove any leading/trailing whitespace from the key
    char *start = key_buffer;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    
    char *end = key_buffer + strlen(key_buffer) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
    
    // Resize the buffer to fit the actual key length
    char *result = (char *)realloc(key_buffer, (end - start + 2) * sizeof(char));
    if (!result) {
        free(key_buffer);
        return NULL;
    }
    
    memmove(result, start, end - start + 2);
    result[end - start + 2] = '\0';
    
    return result;
}
```