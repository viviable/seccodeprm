```c
#include <stdlib.h>
#include <string.h>

char* my_strdup(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str) + 1; // +1 for the null terminator
    char* dup = (char*)malloc(len);
    if (dup == NULL) {
        return NULL; // Memory allocation failed
    }
    strcpy(dup, str);
    return dup;
}
```