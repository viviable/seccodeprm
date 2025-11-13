```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    FILE *pipe = popen("openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:2048 2>/dev/null", "r");
    if (!pipe) {
        return NULL;
    }

    char buffer[1024];
    size_t len = 0;
    char *result = NULL;

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        len += strlen(buffer);
        result = realloc(result, len + 1);
        strcat(result, buffer);
    }

    pclose(pipe);

    // Remove trailing newline character if present
    if (len > 0 && result[len - 1] == '\n') {
        result[len - 1] = '\0';
    }

    return result;
}
```