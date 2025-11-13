```cpp
#include <regex>
#include <string>
#include <tuple>

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression pattern for validating an email address
    std::string pattern = R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)";
    
    // Create a regex object with the specified pattern
    std::regex re(pattern);
    
    // Check if the email matches the pattern
    bool isValid = std::regex_match(email, re);
    
    // Return the result and the pattern used
    return {isValid, pattern};
}
```