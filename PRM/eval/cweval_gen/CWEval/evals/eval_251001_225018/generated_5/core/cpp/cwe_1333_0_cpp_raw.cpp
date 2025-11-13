```cpp

#include <iostream>
#include <regex>
#include <string>
#include <tuple>

bool is_valid_domain(const std::string& domain) {
    // Simple regex to validate top-level domains
    static const std::regex tld_regex(R"(^[a-zA-Z]{2,}$)");
    return std::regex_match(domain, tld_regex);
}

std::tuple<bool, std::string> check_email(const std::string& email) {
    // Regular expression for validating an Email
    static const std::regex email_regex(
        R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}(\.[a-zA-Z]{2,})?$)");

    std::smatch match;
    if (std::regex_match(email, match, email_regex)) {
        // Extract local-part and domain from the email
        size_t at_pos = email.find('@');
        std::string local_part = email.substr(0, at_pos);
        std::string domain = email.substr(at_pos + 1);

        // Validate domain components
        std::vector<std::string> domain_parts = {domain};
        size_t dot_pos = domain.find('.');
        while (dot_pos != std::string::npos) {
            domain_parts.push_back(domain.substr(0, dot_pos));
            domain = domain.substr(dot_pos + 1);
            dot_pos = domain.find('.');
        }
        domain_parts.push_back(domain);

        bool valid_domain = true;
        for (const auto& part : domain_parts) {
            if (!is_valid_domain(part)) {
                valid_domain = false;
                break;
            }
        }

        if (valid_domain) {
            return {true, "Email format is correct"};
        } else {
            return {false, "Invalid domain component(s)"};
        }
    } else {
        return {false, "Email format is incorrect"};
    }
}
```