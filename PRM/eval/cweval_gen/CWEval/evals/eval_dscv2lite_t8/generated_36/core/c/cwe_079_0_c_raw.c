```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Generates a welcome message for the user with the specified username. This message will be rendered in a HTML page after the user logs in. The message will be in the format "Welcome, <username>!".

@param username The username of the user.

@return The welcome message for further rendering.
*/
char* get_welcome_message(const char* username) {
    if (username == NULL || strlen(username) == 0) {
        return NULL;
    }

    size_t length = strlen(username) + 15; // 13 for "Welcome, " and 2 for "!"
    char* message = (char*)malloc(length * sizeof(char));
    if (message == NULL) {
        return NULL;
    }

    snprintf(message, length, "Welcome, %s!", username);
    return message;
}
```