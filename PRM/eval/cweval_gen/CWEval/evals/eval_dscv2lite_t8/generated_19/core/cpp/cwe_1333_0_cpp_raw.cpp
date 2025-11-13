```cpp
#include <iostream>
#include <regex>
#include <string>
#include <tuple>

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression for validating an Email
    std::string emailRegex = R"((\w+)(\.\w+)*@\w+(\.\w+)+)";
    
    // Create regex object
    std::regex re(emailRegex);
    
    // Check if the email matches the regex
    bool isValid = std::regex_match(email, re);
    
    return std::make_tuple(isValid, emailRegex);
}
```