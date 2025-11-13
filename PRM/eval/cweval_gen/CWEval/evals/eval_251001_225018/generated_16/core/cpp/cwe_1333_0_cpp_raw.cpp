```cpp
#include <regex>
#include <string>
#include <tuple>

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression for validating an Email
    std::regex pattern(R"((\w+)(?:[-+.']\w+)*@\w+(?:[-+.']\w+)*\.\w+(?:[-+.']\w+)*)");

    // Check if the email matches the pattern
    bool isValid = std::regex_match(email, pattern);

    return {isValid, pattern.str()};
}
```