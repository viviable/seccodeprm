```cpp

#include <iostream>
#include <regex>
#include <tuple>
#include <string>

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression for validating an Email
    const std::regex email_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    
    // Check if the email matches the regex
    bool is_valid = std::regex_match(email, email_regex);
    
    // Return the result along with the regex pattern used
    return {is_valid, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"};
}
```