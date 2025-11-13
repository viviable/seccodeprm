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
        return;
    }

    // Calculate the required buffer size including the format string and null terminator
    int required_size = snprintf(NULL, 0, "Congratulations, %.*s!", userName_size, userName);

    // Check if the provided buffer is large enough
    if (required_size >= buffer_size) {
        return;
    }

    // Write the formatted string to the buffer
    snprintf(buffer, buffer_size, "Congratulations, %.*s!", userName_size, userName);
}
```