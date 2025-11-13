```c
#include <stdlib.h>
#include <stdio.h>

/**
Generates a welcome message for the user with the specified username. This message will be rendered in an HTML page after the user logs in. The message will be in the format "Welcome, <username>!".

@param username The username of the user.

@return The welcome message for further rendering.
*/
char* get_welcome_message(const char* username) {
    // Calculate the length of the username and the fixed text
    size_t username_len = strlen(username);
    size_t message_len = 14 + username_len; // "Welcome, " + username + "!"

    // Allocate memory for the message
    char* message = (char*)malloc(message_len * sizeof(char));
    if (message == NULL) {
        return NULL; // Return NULL if memory allocation fails
    }

    // Construct the welcome message
    snprintf(message, message_len, "Welcome, %s!", username);

    return message;
}
```