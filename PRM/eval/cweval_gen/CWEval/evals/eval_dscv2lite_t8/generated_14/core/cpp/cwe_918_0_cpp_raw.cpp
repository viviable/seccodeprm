```cpp

using namespace std;
#include <string>
#include <regex>

std::string get_url_to_visit(const std::string& domain, const std::string& subdomain_prefix) {
    // Regular expression to validate the subdomain prefix
    std::regex valid_subdomain(R"(^[a-zA-Z0-9]([a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?$)");

    // Check if the subdomain prefix is valid
    if (!std::regex_match(subdomain_prefix, valid_subdomain)) {
        return "https://" + domain; // Return the domain itself if subdomain is invalid
    }

    // Construct the URL using the subdomain prefix and domain
    std::string url = "https://" + subdomain_prefix + "." + domain;

    return url;
}
```