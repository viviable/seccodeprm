```c
#include <stdlib.h>
#include <string.h>

char* my_strdup(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    char* duplicated_str = (char*)malloc((len + 1) * sizeof(char));
    if (duplicated_str == NULL) {
        return NULL; // Memory allocation failed
    }

    strcpy(duplicated_str, str);
    return duplicated_str;
}
```