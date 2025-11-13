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

    int len = 0;
    while (str[len] != '\0') {
        len++;
    }

    // Find the last non-space character
    int end = len - 1;
    while (end >= 0 && isspace(str[end])) {
        end--;
    }

    // Null-terminate the string at the new end position
    str[end + 1] = '\0';
}
```