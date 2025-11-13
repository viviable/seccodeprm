```cpp

#include <iostream>
#include <regex>
#include <tuple>
#include <string>

bool is_valid_domain(const std::string& domain) {
    std::regex domain_regex(R"(^[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(domain, domain_regex);
}

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression to match the email format
    std::regex email_regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}(\.[a-zA-Z]{2,})?$)");

    if (!std::regex_match(email, email_regex)) {
        return {false, "Email format does not match"};
    }

    size_t at_pos = email.find('@');
    size_t dot_pos = email.rfind('.');
    std::string local_part = email.substr(0, at_pos);
    std::string domain = email.substr(at_pos + 1);

    if (local_part.empty() || domain.empty()) {
        return {false, "Empty local part or domain in email"};
    }

    if (!is_valid_domain(domain)) {
        return {false, "Invalid domain in email"};
    }

    return {true, "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}(\\.[a-zA-Z]{2,})?$"};
}
```