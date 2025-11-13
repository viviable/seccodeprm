```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    char command[128];
    snprintf(command, sizeof(command), "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);

    FILE *pipe = popen(command, "r");
    if (!pipe) {
        return NULL;
    }

    // Read the entire output from the pipe into a buffer
    const int BUFSIZE = 16384;
    char *buffer = (char *)malloc(BUFSIZE);
    if (!buffer) {
        pclose(pipe);
        return NULL;
    }

    size_t bytesRead = fread(buffer, 1, BUFSIZE - 1, pipe);
    buffer[bytesRead] = '\0';

    pclose(pipe);

    // Remove any trailing whitespace
    char *end = buffer + strlen(buffer) - 1;
    while (end > buffer && (*end == '\n' || *end == ' ')) {
        end--;
    }
    *(end + 1) = '\0';

    return buffer;
}
```