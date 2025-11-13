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

    // Calculate the length of the new string
    size_t len = strlen(username);
    size_t total_len = len + 13; // "Welcome, !" is 12 characters long plus null terminator

    // Allocate memory for the new string
    char* message = (char*)malloc(total_len * sizeof(char));
    if (message == NULL) {
        return NULL; // Memory allocation failed
    }

    // Construct the welcome message
    snprintf(message, total_len, "Welcome, %s!", username);

    return message;
}
```