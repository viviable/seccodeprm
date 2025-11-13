```c
#include <stdio.h>
#include <string.h>

/**
Given a user-input string, remove the trailing spaces from the string. The string should be modified in place.
*/
void trim_spaces(char* str) {
    if (str == NULL) {
        return;
    }

    int len = strlen(str);
    if (len == 0) {
        return;
    }

    // Find the last non-space character
    int i;
    for (i = len - 1; i >= 0 && str[i] == ' '; i--) {
        // Do nothing, just decrement the index
    }

    // Null-terminate the string at the first trailing space
    str[i + 1] = '\0';
}
```