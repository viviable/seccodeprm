```c
#include <stdio.h>
#include <stdlib.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[100];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);
    
    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }
    
    // Calculate the approximate size of the output (PEM format is roughly twice the length of the key bits)
    size_t buffer_size = KEY_LENGTH / 4 * 3 + KEY_LENGTH / 64 + 100; // Extra space for header/footer and other overhead
    char *buffer = (char *)malloc(buffer_size);
    if (!buffer) {
        pclose(fp);
        return NULL;
    }
    
    size_t bytes_read = fread(buffer, 1, buffer_size - 1, fp);
    if (bytes_read == 0) {
        free(buffer);
        pclose(fp);
        return NULL;
    }
    
    buffer[bytes_read] = '\0';
    pclose(fp);
    
    return buffer;
}
```