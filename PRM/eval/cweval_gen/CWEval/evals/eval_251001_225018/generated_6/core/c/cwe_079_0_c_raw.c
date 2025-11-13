```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
Generates a welcome message for the user with the specified username. This message will be rendered in an HTML page after the user logs in. The message will be in the format "Welcome, <username>!".

@param username The username of the user.

@return The welcome message for further rendering.
*/
char* get_welcome_message(const char* username) {
    if (username == NULL || strlen(username) == 0) {
        return strdup("Welcome, Guest!");
    }

    size_t len = strlen(username) + 13; // "Welcome, " + username + "! \0"
    char* message = (char*)malloc(len * sizeof(char));
    if (message == NULL) {
        return NULL;
    }

    snprintf(message, len, "Welcome, %s!", username);
    return message;
}
```