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
    int i = len - 1;
    while (i >= 0 && isspace(str[i])) {
        i--;
    }

    // Null-terminate the string at the new end position
    str[i + 1] = '\0';
}
```