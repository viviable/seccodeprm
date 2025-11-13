```cpp
#include <regex>
#include <string>
#include <tuple>

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression for validating an Email
    std::string emailRegex = R"((\w+)(\.\w+)*@\w+(\.\w+)+(\.[a-zA-Z]{2,})?)";
    std::regex re(emailRegex);

    // Check if the email matches the regex
    bool isValid = std::regex_match(email, re);
    return {isValid, emailRegex};
}
```