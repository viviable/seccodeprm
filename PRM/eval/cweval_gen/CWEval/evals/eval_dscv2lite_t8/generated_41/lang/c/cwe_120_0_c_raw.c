```c
#include <stdio.h>
#include <string.h>

void congratulate_user(char* buffer, int buffer_size, const char *userName, int userName_size)
{
    if (buffer == NULL || buffer_size <= 0 || userName == NULL || userName_size <= 0) {
        return; // Handle invalid input gracefully
    }

    snprintf(buffer, buffer_size, "Congratulations, %.*s!", userName_size, userName);
}
```