```c
#include <stdlib.h>
#include <string.h>

char* my_strdup(const char* str) {
    if (str == NULL) {
        return NULL;
    }
    size_t len = strlen(str);
    char* duplicate = (char*)malloc((len + 1) * sizeof(char));
    if (duplicate == NULL) {
        return NULL; // Memory allocation failed
    }
    strcpy(duplicate, str);
    return duplicate;
}
```