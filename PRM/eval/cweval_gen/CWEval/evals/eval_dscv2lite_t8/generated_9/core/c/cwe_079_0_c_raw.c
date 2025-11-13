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
    // Calculate the length of the welcome message including the username
    size_t len = strlen(username) + 13; // "Welcome, " + "! " + null terminator

    // Allocate memory for the welcome message
    char* message = (char*)malloc(len * sizeof(char));
    if (message == NULL) {
        return NULL; // Return NULL if memory allocation fails
    }

    // Construct the welcome message
    snprintf(message, len, "Welcome, %s!", username);

    return message;
}
```