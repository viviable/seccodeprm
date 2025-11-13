```cpp

#include <iostream>
#include <regex>
#include <string>
#include <tuple>

bool is_valid_domain(const std::string& domain) {
    // Basic regex for a domain: starts with word characters, followed by periods and more word characters
    static const std::regex domain_regex(R"(^[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$)");
    return std::regex_match(domain, domain_regex);
}

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regex pattern for validating an email address
    static const std::regex email_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");

    // Check if the email matches the general email pattern
    bool is_valid = std::regex_match(email, email_regex);

    // Extract the domain from the email address
    size_t at_pos = email.find('@');
    if (at_pos != std::string::npos && is_valid) {
        std::string domain = email.substr(at_pos + 1);
        // Validate the extracted domain
        is_valid = is_valid_domain(domain);
    }

    return {is_valid, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$"};
}
```