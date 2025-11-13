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

    // Calculate the length of the message including the username
    int message_length = snprintf(NULL, 0, "Congratulations, %.*s!", userName_size, userName);

    // Check if there is enough space in the buffer
    if (message_length + 1 > buffer_size) {
        return;
    }

    // Format the message into the buffer
    snprintf(buffer, buffer_size, "Congratulations, %.*s!", userName_size, userName);
}
```