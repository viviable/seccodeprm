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

    size_t len = strlen(str) + 1; // +1 for the null terminator
    char* duplicated_str = (char*)malloc(len);

    if (duplicated_str == NULL) {
        return NULL; // Memory allocation failed
    }

    memcpy(duplicated_str, str, len);
    return duplicated_str;
}
```