```cpp

#include <iostream>
#include <regex>
#include <tuple>
#include <string>

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression for validating an Email
    std::regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");

    // Check if the email matches the pattern
    bool isValid = std::regex_match(email, pattern);

    // Return the result along with the used pattern
    return {isValid, pattern.str()};
}
```