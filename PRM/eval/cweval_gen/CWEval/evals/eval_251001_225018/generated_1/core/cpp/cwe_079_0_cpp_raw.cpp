```cpp

#include <iostream>
#include <string>

// Generates a welcome message for the user with the specified username. This message will be rendered on an HTML page after the user logs in. The message will be in the format "Welcome, <username>!".
//
// @param username The username of the user.
// @return The welcome message for further rendering.
std::string get_welcome_message(const std::string &username) {
    return "Welcome, " + username + "!";
}
```