```c
#include <stdlib.h>
#include <string.h>

char* my_strdup(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str) + 1; // +1 for the null terminator
    char* duplicate = (char*)malloc(len * sizeof(char));
    if (duplicate == NULL) {
        return NULL; // Memory allocation failed
    }

    strcpy(duplicate, str);
    return duplicate;
}
```