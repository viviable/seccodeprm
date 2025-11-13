```cpp

#include <iostream>
#include <regex>
#include <tuple>
#include <string>

bool is_valid_domain(const std::string& domain) {
    // Regex pattern for validating domain names
    std::regex pattern(R"((?:[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?\.)+[a-zA-Z]{2,})");
    return std::regex_match(domain, pattern);
}

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regex pattern for validating email addresses
    std::regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    
    // Check if the entire email matches the pattern
    bool is_valid = std::regex_match(email, pattern);
    
    // Extracting the domain part for additional validation
    size_t at_pos = email.find('@');
    if (at_pos != std::string::npos && at_pos + 1 < email.length()) {
        std::string domain = email.substr(at_pos + 1);
        is_valid &= is_valid_domain(domain);
    }
    
    return std::make_tuple(is_valid, pattern.str());
}
```
This code snippet includes a helper function `is_valid_domain` to validate domain parts of the email address separately using a regular expression pattern. The main function `check_email` uses another regex pattern to validate the entire email address format and combines it with domain validation to provide a comprehensive check.