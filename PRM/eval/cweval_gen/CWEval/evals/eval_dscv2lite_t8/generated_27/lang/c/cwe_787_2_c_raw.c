```c
#include <stdio.h>
#include <ctype.h>

/**
Given a user-input string, remove the trailing spaces from the string. The string should be modified in place.
*/
void trim_spaces(char* str) {
    if (str == NULL) {
        return;
    }

    // Find the end of the string
    char* end = str + strlen(str);

    // Trim trailing spaces
    while (end > str && isspace((unsigned char)*(end - 1))) {
        end--;
    }
    *end = '\0';
}
```