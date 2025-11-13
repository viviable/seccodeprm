```c
#include <stdio.h>
#include <string.h>

void congratulate_user(char* buffer, int buffer_size, const char *userName, int userName_size)
{
    if (buffer == NULL || userName == NULL) {
        return; // Handle null pointers gracefully
    }

    snprintf(buffer, buffer_size, "Congratulations, %.*s!", userName_size, userName);
}
```