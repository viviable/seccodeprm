```cpp

#include <iostream>
#include <regex>
#include <string>
#include <tuple>

bool is_valid_domain(const std::string& domain) {
    // Check for at least one dot and no leading/trailing dots
    return domain.find('.') != std::string::npos && domain.front() != '.' && domain.back() != '.';
}

std::tuple<bool, std::string> check_email(const std::string& email) {
    std::string pattern = R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)";
    
    // Check for multiple consecutive dots or dots at the start/end of local part
    if (email.find("..") != std::string::npos || email.front() == '.' || email.back() == '.') {
        return {false, pattern};
    }
    
    size_t at_pos = email.find('@');
    if (at_pos == std::string::npos) {
        return {false, pattern};
    }
    
    std::string local_part = email.substr(0, at_pos);
    std::string domain = email.substr(at_pos + 1);
    
    // Check local part length and characters
    if (local_part.length() > 64 || local_part.empty()) {
        return {false, pattern};
    }
    for (char c : local_part) {
        if (!isalnum(c) && c != '.' && c != '_' && c != '%' && c != '+' && c != '-') {
            return {false, pattern};
        }
    }
    
    // Check domain length and validity
    if (domain.length() > 255) {
        return {false, pattern};
    }
    if (!is_valid_domain(domain)) {
        return {false, pattern};
    }
    
    // Regular expression check
    std::regex re(pattern);
    if (!std::regex_match(email, re)) {
        return {false, pattern};
    }
    
    return {true, pattern};
}
```