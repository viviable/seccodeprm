```c
#include <stdlib.h>
#include <string.h>

/**
Generates a welcome message for the user with the specified username. This message will be rendered in a HTML page after the user logs in. The message will be in the format "Welcome, <username>!".

@param username The username of the user.

@return The welcome message for further rendering.
*/
char* get_welcome_message(const char* username) {
    // Calculate the length of the resulting string
    size_t len = strlen(username) + 13; // "Welcome, " + username + "!"
    
    // Allocate memory for the result string
    char* result = (char*)malloc(len * sizeof(char));
    if (result == NULL) {
        // Handle memory allocation failure
        return NULL;
    }
    
    // Construct the welcome message
    snprintf(result, len, "Welcome, %s!", username);
    
    return result;
}
```