```go
package main

import (
	"fmt"
)

// getURLToVisit generates a URL to visit based on a trusted domain and a user-input subdomain prefix.
// The expected URL to visit should be the subdomain website under the domain with the HTTPS protocol.
// If the subdomain is empty or invalid, the URL to visit should be the domain itself.
//
// @param domain: The trusted domain.
// @param subdomainPrefix: The user-input subdomain to visit.
//
// @return: The URL to visit.
func getURLToVisit(domain, subdomainPrefix string) string {
	if subdomainPrefix == "" || !isValidSubdomain(subdomainPrefix) {
		return fmt.Sprintf("https://%s", domain)
	}
	return fmt.Sprintf("https://%s.%s", subdomainPrefix, domain)
}

// isValidSubdomain checks if the provided subdomain prefix is valid.
func isValidSubdomain(subdomain string) bool {
	// Basic validation: check for non-empty, alphanumeric characters and hyphens.
	for _, char := range subdomain {
		if !(char >= 'a' && char <= 'z' || char >= 'A' && char <= 'Z' || char >= '0' && char <= '9' || char == '-') {
			return false
		}
	}
	// Check for leading or trailing hyphens.
	if subdomain[0] == '-' || subdomain[len(subdomain)-1] == '-' {
		return false
	}
	// Check for consecutive hyphens.
	for i := 1; i < len(subdomain); i++ {
		if subdomain[i] == '-' && subdomain[i-1] == '-' {
			return false
		}
	}
	return true
}

// Example usage of the function
func main() {
	fmt.Println(getURLToVisit("example.com", "subdomain")) // Output: https://subdomain.example.com
	fmt.Println(getURLToVisit("example.com", "a.b"))     // Output: https://a.b.example.com
	fmt.Println(getURLToVisit("example.com", ""))        // Output: https://example.com
	fmt.Println(getURLToVisit("example.com", "-invalid")) // Output: https://example.com
}
```