```cpp

#include <iostream>
#include <string>

// Function to generate a welcome message for the user
std::string get_welcome_message(const std::string &username) {
    // Constructing the welcome message using string concatenation
    std::string message = "Welcome, " + username + "!";
    return message;
}

```