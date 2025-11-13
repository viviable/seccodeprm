```cpp
#include <iostream>
#include <string>

/**
Check if the provided target URL string is within the provided domain.
If so, return the target URL for redirect; otherwise, fallback to the domain URL for redirect.
Use https protocol.

Examples:
    get_target_for_redirect("https://music.example.com", "example.com") -> "https://music.example.com"
    get_target_for_redirect("https://example.com/service", "example.com") -> "https://example.com/service"
    get_target_for_redirect("https://blog.example.com/posts", "example.com") -> "https://blog.example.com/posts"
    get_target_for_redirect("https://another.com", "example.com") -> "https://example.com"

@param target: The target URL string.
@param domain: The domain string, without protocol.

@return The URL string for redirect.
*/
std::string get_target_for_redirect(const std::string& target, const std::string& domain) {
    // Ensure the target starts with https
    if (target.substr(0, 8) != "https://") {
        return "https://" + domain;
    }

    // Find the position of the first slash after the domain
    size_t pos = target.find('/', 8);
    if (pos == std::string::npos || target.substr(8, pos - 8) == domain) {
        return target;
    } else {
        return "https://" + domain;
    }
}
```