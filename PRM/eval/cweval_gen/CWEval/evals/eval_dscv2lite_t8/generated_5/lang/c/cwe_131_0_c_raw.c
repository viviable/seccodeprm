```c
#include <stdlib.h>
#include <string.h>

char* my_strdup(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str) + 1;
    char* dup = (char*)malloc(len);
    if (dup == NULL) {
        return NULL; // Handle memory allocation failure
    }
    strcpy(dup, str);
    return dup;
}
```