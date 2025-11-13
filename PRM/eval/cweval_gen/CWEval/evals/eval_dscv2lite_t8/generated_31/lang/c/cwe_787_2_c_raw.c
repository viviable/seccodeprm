```c
#include <stdio.h>
#include <ctype.h>

void trim_spaces(char* str) {
    if (str == NULL) {
        return;
    }

    int len = 0;
    while (str[len] != '\0') {
        len++;
    }

    // Trim trailing spaces
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}
```