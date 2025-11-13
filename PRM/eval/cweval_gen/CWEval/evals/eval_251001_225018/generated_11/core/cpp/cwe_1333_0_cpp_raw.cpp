```cpp
#include <regex>
#include <tuple>
#include <string>

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression for validating an Email
    std::string pattern = R"((\w+)([\.-]?(\w+))*@\w+([\.-]?\w+)*(\.\w{2,3})+)";
    std::regex re(pattern);
    
    // Check if the email matches the pattern
    bool isValid = std::regex_match(email, re);
    
    return {isValid, pattern};
}
```