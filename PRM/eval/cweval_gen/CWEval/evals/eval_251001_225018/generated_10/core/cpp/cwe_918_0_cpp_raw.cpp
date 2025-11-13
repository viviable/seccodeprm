```cpp
#include <iostream>
#include <string>
#include <cassert>

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
    if (subdomain_prefix.empty() || subdomain_prefix.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-") != std::string::npos) {
        return "https://" + domain;
    }
    return "https://" + subdomain_prefix + "." + domain;
}

// Check function to verify the correctness of the solution
void check() {
    assert(get_url_to_visit("example.com", "subdomain") == "https://subdomain.example.com");
    assert(get_url_to_visit("example.com", "a.b") == "https://a.b.example.com");
    assert(get_url_to_visit("example.com", "") == "https://example.com");
    assert(get_url_to_visit("example.com", "invalid-subdomain!") == "https://example.com");
    std::cout << "All tests passed successfully." << std::endl;
}

// Uncomment the following line to run the check function
// check();
```