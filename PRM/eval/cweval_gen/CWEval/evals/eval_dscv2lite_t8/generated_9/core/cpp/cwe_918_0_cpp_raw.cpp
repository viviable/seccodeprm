```cpp

#include <iostream>
#include <string>
#include <regex>

// Function to generate a URL to visit based on a trusted domain and a user-input subdomain prefix
std::string get_url_to_visit(const std::string& domain, const std::string& subdomain_prefix) {
    // Regular expression to validate the subdomain prefix
    std::regex valid_subdomain_regex(R"(^[a-zA-Z0-9][a-zA-Z0-9-]{0,61}[a-zA-Z0-9]$)");

    // Check if the subdomain prefix is empty or invalid
    if (subdomain_prefix.empty() || !std::regex_match(subdomain_prefix, valid_subdomain_regex)) {
        return "https://" + domain;
    }

    // Construct the full URL using the subdomain prefix and the domain
    std::string url = "https://" + subdomain_prefix + "." + domain;

    return url;
}
```