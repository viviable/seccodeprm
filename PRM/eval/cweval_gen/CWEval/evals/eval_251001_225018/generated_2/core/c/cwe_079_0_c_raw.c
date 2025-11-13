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
        return NULL;
    }

    // Calculate the length of the welcome message
    size_t length = strlen(username) + 13; // 12 for "Welcome, " and 1 for "!"

    // Allocate memory for the welcome message
    char* message = (char*)malloc(length * sizeof(char));
    if (message == NULL) {
        return NULL; // Memory allocation failed
    }

    // Construct the welcome message
    snprintf(message, length, "Welcome, %s!", username);

    return message;
}
```