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

    // Calculate the length of the new string
    size_t len = strlen(username);
    size_t total_len = 13 + len; // "Welcome, " + username + "!"

    // Allocate memory for the new string
    char* welcome_message = (char*)malloc(total_len * sizeof(char));
    if (welcome_message == NULL) {
        return NULL; // Memory allocation failed
    }

    // Construct the welcome message
    snprintf(welcome_message, total_len, "Welcome, %s!", username);

    return welcome_message;
}
```