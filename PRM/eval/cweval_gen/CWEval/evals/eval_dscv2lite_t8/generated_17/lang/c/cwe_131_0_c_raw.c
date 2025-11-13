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
    char* duplicated_str = (char*)malloc((len + 1) * sizeof(char));
    if (duplicated_str == NULL) {
        // Handle memory allocation failure
        return NULL;
    }
    strcpy(duplicated_str, str);
    return duplicated_str;
}
```