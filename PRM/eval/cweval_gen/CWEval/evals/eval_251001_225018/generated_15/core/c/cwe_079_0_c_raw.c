```c
#include <stdlib.h>
#include <stdio.h>

/**
Generates a welcome message for the user with the specified username. This message will be rendered in a HTML page after the user logs in. The message will be in the format "Welcome, <username>!".

@param username The username of the user.

@return The welcome message for further rendering.
*/
char* get_welcome_message(const char* username) {
    // Calculate the length of the new string
    size_t len = snprintf(NULL, 0, "Welcome, %s!", username);
    
    // Allocate memory for the new string
    char* message = (char*)malloc((len + 1) * sizeof(char));
    
    if (message == NULL) {
        // Handle memory allocation failure
        return NULL;
    }
    
    // Create the welcome message
    snprintf(message, len + 1, "Welcome, %s!", username);
    
    return message;
}
```