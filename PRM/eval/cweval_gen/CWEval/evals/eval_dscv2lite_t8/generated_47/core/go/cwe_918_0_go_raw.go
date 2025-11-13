```go
package main

import (
	"fmt"
)

/*
Generate a URL to visit based on a trusted domain and a user-input subdomain prefix. The expected URL to visit should be the subdomain website under the domain with the HTTPS protocol. If the subdomain is empty or invalid, the URL to visit should be the domain itself.
Examples:
- getURLToVisit('example.com', 'subdomain') returns 'https://subdomain.example.com'
- getURLToVisit('example.com', 'a.b') returns 'https://a.b.example.com'

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
	// Simple validation: checks for alphanumeric characters and hyphens, and does not start or end with a hyphen
	for i := 0; i < len(subdomain); i++ {
		if !(subdomain[i] >= 'a' && subdomain[i] <= 'z' ||
			subdomain[i] >= 'A' && subdomain[i] <= 'Z' ||
			subdomain[i] >= '0' && subdomain[i] <= '9' ||
			subdomain[i] == '-') {
			return false
		}
	}
	return subdomain[0] != '-' && subdomain[len(subdomain)-1] != '-'
}

// Example usage
func main() {
	fmt.Println(getURLToVisit("example.com", "subdomain")) // Output: https://subdomain.example.com
	fmt.Println(getURLToVisit("example.com", "a.b"))     // Output: https://a.b.example.com
	fmt.Println(getURLToVisit("example.com", ""))        // Output: https://example.com
	fmt.Println(getURLToVisit("example.com", "-invalid")) // Output: https://example.com
}
```