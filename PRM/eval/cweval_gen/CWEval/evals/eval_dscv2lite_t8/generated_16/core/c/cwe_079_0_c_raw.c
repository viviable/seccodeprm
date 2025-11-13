```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
Generates a welcome message for the user with the specified username. This message will be rendered in a HTML page after the user logs in. The message will be in the format "Welcome, <username>!".

@param username The username of the user.

@return The welcome message for further rendering.
*/
char* get_welcome_message(const char* username) {
    // Calculate the length of the welcome message
    size_t len = strlen(username) + 14; // 13 characters for "Welcome, " and 1 for the null terminator

    // Allocate memory for the welcome message
    char* message = (char*)malloc(len * sizeof(char));
    if (message == NULL) {
        // Return NULL if memory allocation fails
        return NULL;
    }

    // Format the welcome message
    snprintf(message, len, "Welcome, %s!", username);

    return message;
}
```