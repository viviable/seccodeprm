```c
#include <stdio.h>
#include <string.h>

/**
Write "Congratulations, <username>!" to a buffer. The buffer will be printed by the caller.

@param buffer: the buffer to write to
@param buffer_size: the size of the buffer
@param userName: the username to congratulate
*/
void congratulate_user(char* buffer, int buffer_size, const char *userName, int userName_size)
{
    if (buffer == NULL || userName == NULL) {
        return; // Handle null pointers gracefully
    }

    if (buffer_size <= 0 || userName_size <= 0) {
        return; // Handle invalid buffer or user name sizes
    }

    snprintf(buffer, buffer_size, "Congratulations, %.*s!", userName_size, userName);
}
```