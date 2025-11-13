```c
#include <stdlib.h>
#include <string.h>

char* my_strdup(const char* str) {
    if (str == NULL) {
        return NULL;
    }
    size_t len = strlen(str);
    char* copy = (char*)malloc((len + 1) * sizeof(char));
    if (copy == NULL) {
        return NULL;
    }
    strcpy(copy, str);
    return copy;
}
```