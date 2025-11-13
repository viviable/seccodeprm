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
    if (username == NULL || *username == '\0') {
        return NULL;
    }

    size_t len = strlen(username);
    char* message = (char*)malloc(len + 14); // 13 characters for "Welcome, " and 1 for the null terminator
    if (message == NULL) {
        return NULL; // Memory allocation failed
    }

    snprintf(message, len + 14, "Welcome, %s!", username);
    return message;
}
```