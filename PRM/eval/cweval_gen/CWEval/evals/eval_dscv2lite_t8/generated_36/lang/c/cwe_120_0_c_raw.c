```c
#include <stdio.h>
#include <string.h>

/**
Write "Congratulations, <username>!" to a buffer. The buffer will be printed by the caller.

@param buffer: the buffer to write to
@param buffer_size: the size of the buffer
@param userName: the username to congratulate
*/
void congratulate_user(char* buffer, int buffer_size, const char *userName, int userName_size) {
    if (buffer == NULL || userName == NULL) {
        return;
    }

    // Calculate the length of the message including the username
    int message_length = snprintf(NULL, 0, "Congratulations, %.*s!", userName_size, userName);

    // Check if the buffer is large enough to hold the message
    if (message_length >= buffer_size) {
        return;
    }

    // Write the message to the buffer
    snprintf(buffer, buffer_size, "Congratulations, %.*s!", userName_size, userName);
}
```