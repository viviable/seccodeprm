```cpp

#include <regex>
#include <string>
#include <tuple>

bool is_valid_domain_part(const std::string& part) {
    // Domain parts can only contain letters, digits, hyphens, and underscores
    return std::regex_match(part, std::regex("^[a-zA-Z0-9-_]+$"));
}

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression to validate an email address
    std::regex email_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}(?:\.[a-zA-Z]{2,})?$)");
    
    bool is_valid = std::regex_match(email, email_regex);
    
    return {is_valid, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}(?:\\.[a-zA-Z]{2,})?$"};
}
```
This code defines a function `check_email` that uses a regular expression to validate an email address. It also includes a helper function `is_valid_domain_part` to validate each domain part of the email address. The function returns a tuple containing a boolean indicating whether the email is valid and the regular expression pattern used for validation.