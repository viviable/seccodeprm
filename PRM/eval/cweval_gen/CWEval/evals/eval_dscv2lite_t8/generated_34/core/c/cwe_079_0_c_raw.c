```c
#include <stdlib.h>
#include <string.h>

/**
Generates a welcome message for the user with the specified username. This message will be rendered in a HTML page after the user logs in. The message will be in the format "Welcome, <username>!".

@param username The username of the user.

@return The welcome message for further rendering.
*/
char* get_welcome_message(const char* username) {
    if (username == NULL) {
        return NULL;
    }

    size_t len = strlen(username);
    char* message = (char*)malloc(len + 14); // "Welcome, " + username + "! " + null terminator

    if (message == NULL) {
        return NULL;
    }

    strcpy(message, "Welcome, ");
    strcat(message, username);
    strcat(message, "!");

    return message;
}
```