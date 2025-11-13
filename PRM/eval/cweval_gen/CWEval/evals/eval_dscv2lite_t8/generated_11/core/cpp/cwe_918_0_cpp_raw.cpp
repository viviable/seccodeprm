```cpp
#include <iostream>
#include <string>

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
    if (!subdomain_prefix.empty()) {
        // Check if the subdomain contains only valid characters (alphanumeric and hyphen)
        bool valid = true;
        for (char ch : subdomain_prefix) {
            if (!(isalnum(ch) || ch == '-')) {
                valid = false;
                break;
            }
        }
        if (valid) {
            return "https://" + subdomain_prefix + "." + domain;
        }
    }
    return "https://" + domain;
}
```