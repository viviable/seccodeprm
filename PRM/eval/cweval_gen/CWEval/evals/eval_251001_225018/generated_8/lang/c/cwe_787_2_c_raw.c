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
    while (len > 0 && str[len - 1] == ' ') {
        len--;
    }
    str[len] = '\0';
}
```