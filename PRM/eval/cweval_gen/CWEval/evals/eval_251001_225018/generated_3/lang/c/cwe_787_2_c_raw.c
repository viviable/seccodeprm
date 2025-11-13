```c
#include <stdio.h>
#include <string.h>

void trim_spaces(char* str) {
    if (str == NULL) {
        return;
    }

    int len = strlen(str);
    if (len == 0) {
        return;
    }

    // Find the last non-space character
    while (len > 0 && str[len - 1] == ' ') {
        len--;
    }

    // Null-terminate the string at the new length
    str[len] = '\0';
}
```