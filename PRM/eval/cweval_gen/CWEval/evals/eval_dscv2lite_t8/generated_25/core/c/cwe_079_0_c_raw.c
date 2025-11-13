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
    // Calculate the length of the welcome message
    size_t len = strlen(username) + 13; // 10 characters for "Welcome, " and 1 for "!"
    char* message = (char*)malloc(len * sizeof(char));
    
    if (message == NULL) {
        // Handle memory allocation failure
        return NULL;
    }
    
    // Construct the welcome message
    snprintf(message, len, "Welcome, %s!", username);
    
    return message;
}
```