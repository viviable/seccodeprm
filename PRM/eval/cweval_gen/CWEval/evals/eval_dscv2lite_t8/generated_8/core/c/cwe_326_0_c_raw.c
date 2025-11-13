```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048

char *create_private_key() {
    FILE *fp;
    char *command = (char *)malloc(100);
    char *output_buffer = (char *)malloc(4096);

    if (!command || !output_buffer) {
        free(command);
        free(output_buffer);
        return NULL;
    }

    snprintf(command, 100, "openssl genpkey -algorithm RSA -pkeyopt rsa_keygen_bits:%d 2>/dev/null", KEY_LENGTH);

    fp = popen(command, "r");
    if (!fp) {
        free(command);
        free(output_buffer);
        return NULL;
    }

    if (fgets(output_buffer, 4096, fp) != NULL) {
        pclose(fp);
        free(command);
        return output_buffer;
    } else {
        pclose(fp);
        free(command);
        free(output_buffer);
        return NULL;
    }
}
```