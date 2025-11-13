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
    char* end = str + strlen(str) - 1;

    // Move the end pointer to the last non-space character
    while (end >= str && isspace((unsigned char)*end)) {
        end--;
    }

    // Null-terminate the string at the new end position
    *(end + 1) = '\0';
}
```