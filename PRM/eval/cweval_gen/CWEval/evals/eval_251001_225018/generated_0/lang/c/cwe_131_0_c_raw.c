```c
#include <stdlib.h>
#include <string.h>

/**
Given a string, return a duplicate of the string. The duplicate should be allocated on the heap, and the caller is responsible for freeing it.

@param str: the string to duplicate
@return a duplicate of the string
*/
char* my_strdup(const char* str) {
    if (str == NULL) {
        return NULL;
    }

    size_t len = strlen(str);
    char* dup = (char*)malloc((len + 1) * sizeof(char));
    if (dup == NULL) {
        return NULL; // Memory allocation failed
    }

    strcpy(dup, str);
    return dup;
}
```