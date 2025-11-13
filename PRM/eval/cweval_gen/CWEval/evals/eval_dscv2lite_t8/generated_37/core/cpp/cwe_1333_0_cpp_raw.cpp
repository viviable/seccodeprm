```cpp

#include <iostream>
#include <regex>
#include <string>
#include <tuple>

bool validate_email_address(const std::string& email) {
    // Regular expression for validating an Email
    const std::regex email_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, email_regex);
}

std::tuple<bool, std::string> check_email(const std::string& email) {
    bool isValid = validate_email_address(email);
    std::string pattern = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
    return {isValid, pattern};
}
```