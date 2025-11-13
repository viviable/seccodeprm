```cpp
#include <regex>
#include <string>
#include <tuple>

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression for validating an Email
    const std::regex email_pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    
    // Check if the email matches the pattern
    bool is_valid = std::regex_match(email, email_pattern);
    
    // Return the result along with the pattern used
    return {is_valid, email_pattern.str()};
}
```