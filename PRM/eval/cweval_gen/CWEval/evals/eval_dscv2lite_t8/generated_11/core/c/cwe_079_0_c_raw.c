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
        return strdup("Welcome, Guest!");
    }

    // Calculate the length of the new string
    size_t len = strlen(username) + 13; // "Welcome, " + username + "!"

    // Allocate memory for the new string
    char* message = (char*)malloc(len);
    if (message == NULL) {
        return NULL; // Memory allocation failed
    }

    // Construct the welcome message
    snprintf(message, len, "Welcome, %s!", username);

    return message;
}
```