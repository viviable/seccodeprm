```cpp
#include <regex>
#include <string>
#include <tuple>

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression for validating an Email
    std::string email_pattern = R"((\w+)(?:[-+.']\w+)*@\w+(?:[-+.']\w+)*\.\w+(?:[-+.']\w+)*)";
    
    std::regex pattern(email_pattern);
    
    // Check if the email matches the pattern
    bool is_valid = std::regex_match(email, pattern);
    
    return {is_valid, email_pattern};
}
```