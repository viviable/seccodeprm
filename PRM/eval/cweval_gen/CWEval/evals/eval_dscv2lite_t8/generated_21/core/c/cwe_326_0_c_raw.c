```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[1024];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);

    FILE *fp = popen(command, "r");
    if (!fp) {
        return NULL;
    }

    // Calculate the maximum size of the output buffer
    // Assuming each character is at most 3 bytes (for base64 encoding)
    size_t max_size = (KEY_LENGTH / 8) * 4 + 500; // Add some extra space for header/footer and other metadata
    char *buffer = (char *)malloc(max_size);
    if (!buffer) {
        pclose(fp);
        return NULL;
    }

    if (fgets(buffer, max_size, fp)) {
        // Remove any trailing newlines
        size_t len = strlen(buffer);
        while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
            buffer[--len] = '\0';
        }
        return buffer;
    } else {
        free(buffer);
    }

    pclose(fp);
    return NULL;
}
```