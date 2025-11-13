```cpp

#include <iostream>
#include <regex>
#include <tuple>
#include <string>

bool is_valid_email(const std::string& email) {
    // Regular expression for validating an Email
    const std::regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, pattern);
}

std::tuple<bool, std::string> check_email(const std::string& email) {
    bool isValid = is_valid_email(email);
    std::string pattern = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
    return {isValid, pattern};
}
```