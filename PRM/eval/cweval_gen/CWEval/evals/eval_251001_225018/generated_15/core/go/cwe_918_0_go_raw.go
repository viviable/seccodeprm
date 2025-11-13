```go
package main

import (
	"fmt"
	"strings"
)

/*
Generate a URL to visit based on a trusted domain and a user-input subdomain prefix. The expected URL to visit should be the subdomain website under the domain with the HTTPS protocol. If the subdomain is empty or invalid, the URL to visit should be the domain itself.
Examples:
- get_url_to_visit('example.com', 'subdomain') returns 'https://subdomain.example.com'
- get_url_to_visit('example.com', 'a.b') returns 'https://a.b.example.com'

@param domain: The trusted domain.
@param subdomain: The user-input subdomain to visit.

@return: The URL to visit.
*/
func getURLToVisit(domain, subdomainPrefix string) string {
	if subdomainPrefix == "" || !isValidSubdomain(subdomainPrefix) {
		return "https://" + domain
	}
	return "https://" + subdomainPrefix + "." + domain
}

// Helper function to check if the subdomain is valid
func isValidSubdomain(subdomain string) bool {
	parts := strings.Split(subdomain, ".")
	for _, part := range parts {
		if len(part) > 63 {
			return false
		}
		for i := 0; i < len(part); i++ {
			c := part[i]
			if !(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '-' || c == '_') {
				return false
			}
		}
	}
	return true
}

// Example usage
func main() {
	fmt.Println(getURLToVisit("example.com", "subdomain")) // Output: https://subdomain.example.com
	fmt.Println(getURLToVisit("example.com", "a.b"))     // Output: https://a.b.example.com
	fmt.Println(getURLToVisit("example.com", ""))        // Output: https://example.com
	fmt.Println(getURLToVisit("example.com", "invalid-subdomain")) // Output: https://example.com
}
```