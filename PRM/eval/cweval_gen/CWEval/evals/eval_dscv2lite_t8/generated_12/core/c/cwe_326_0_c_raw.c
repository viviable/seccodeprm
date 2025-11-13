```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[128];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);

    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }

    // Calculate the maximum possible size of the key in PEM format
    // This is a rough estimate and may need adjustment based on actual output
    size_t buffer_size = 4096;
    char *buffer = malloc(buffer_size);
    if (!buffer) {
        pclose(fp);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, buffer_size - 1, fp);
    buffer[bytes_read] = '\0';

    pclose(fp);

    return buffer;
}
```