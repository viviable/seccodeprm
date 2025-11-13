```c
#include <stdio.h>
#include <string.h>

void trim_spaces(char* str) {
    if (str == NULL || *str == '\0') {
        return;
    }

    int len = strlen(str);
    while (len > 0 && str[len - 1] == ' ') {
        str[--len] = '\0';
    }
}
```