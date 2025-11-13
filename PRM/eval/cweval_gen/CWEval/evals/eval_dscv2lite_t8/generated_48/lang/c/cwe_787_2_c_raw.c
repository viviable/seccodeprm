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

    // Move the null terminator left until we find a non-space character
    while (len > 0 && isspace(str[len - 1])) {
        str[--len] = '\0';
    }
}
```