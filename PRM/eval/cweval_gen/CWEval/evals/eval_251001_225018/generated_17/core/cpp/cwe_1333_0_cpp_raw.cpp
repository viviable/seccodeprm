```cpp
#include <iostream>
#include <regex>
#include <string>
#include <tuple>

bool is_valid_domain(const std::string& domain) {
    // Simple regex for domain validation
    static const std::regex domain_regex(R"(^[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(domain, domain_regex);
}

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regex for validating an email address
    static const std::regex email_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");

    std::smatch match;
    bool isValid = std::regex_match(email, match, email_regex);

    if (isValid) {
        // Extracting domain from the email
        size_t atIndex = email.find('@');
        std::string domain = email.substr(atIndex + 1);
        isValid = is_valid_domain(domain);
    }

    return {isValid, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"};
}
```