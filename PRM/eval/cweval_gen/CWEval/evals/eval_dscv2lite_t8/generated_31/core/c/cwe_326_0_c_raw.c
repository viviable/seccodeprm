```c
#include <stdio.h>
#include <stdlib.h>

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
    
    // Estimate the size of the key output based on the key length
    size_t estimated_size = (key_length / 8) * 1.3 + 100; // Add some extra space for header and footer
    char *private_key = (char *)malloc(estimated_size);
    if (!private_key) {
        pclose(fp);
        return NULL;
    }
    
    size_t total_read = 0;
    while (fgets(private_key + total_read, estimated_size - total_read, fp)) {
        total_read += strlen(private_key + total_read);
    }
    
    pclose(fp);
    
    // Null-terminate the string
    private_key[total_read] = '\0';
    
    // Resize the buffer to exactly fit the key
    private_key = realloc(private_key, total_read + 1);
    if (!private_key) {
        return NULL;
    }
    
    return private_key;
}
```