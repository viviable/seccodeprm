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
    // Calculate the length of the username and the fixed string parts
    size_t len = strlen(username) + 14; // 13 characters for "Welcome, " and "!" plus 1 for null terminator

    // Allocate memory for the new string
    char* message = (char*)malloc(len * sizeof(char));
    if (message == NULL) {
        // Return NULL if memory allocation fails
        return NULL;
    }

    // Construct the welcome message
    snprintf(message, len, "Welcome, %s!", username);

    // Return the constructed message
    return message;
}
```