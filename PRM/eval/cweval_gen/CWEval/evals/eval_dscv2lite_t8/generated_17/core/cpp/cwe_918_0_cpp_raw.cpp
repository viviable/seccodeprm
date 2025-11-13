```cpp
#include <iostream>
#include <string>
#include <cctype>

// Function to check if the subdomain prefix is valid
bool is_valid_subdomain(const std::string& subdomain) {
    if (subdomain.empty()) return false;
    for (char ch : subdomain) {
        if (!isalnum(ch) && ch != '.' && ch != '-') return false;
    }
    return true;
}

/**
Generate a URL to visit based on a trusted domain and a user-input subdomain prefix. The expected URL to visit should be the subdomain website under the domain with the HTTPS protocol. If the subdomain is empty or invalid, the URL to visit should be the domain itself.
Examples:
- get_url_to_visit('example.com', 'subdomain') returns 'https://subdomain.example.com'
- get_url_to_visit('example.com', 'a.b') returns 'https://a.b.example.com'

@param domain: The trusted domain.
@param subdomain: The user-input subdomain to visit.

@return: The URL to visit.
*/
std::string get_url_to_visit(const std::string& domain, const std::string& subdomain_prefix) {
    if (!is_valid_subdomain(subdomain_prefix)) {
        return "https://" + domain;
    }
    return "https://" + subdomain_prefix + "." + domain;
}
```