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
    int i;
    for (i = len - 1; i >= 0; --i) {
        if (str[i] != ' ') {
            break;
        }
    }

    // Null-terminate the string at the first space found from the end
    str[i + 1] = '\0';
}
```