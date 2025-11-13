```go
package main

import (
	"fmt"
)

// Generate a URL to visit based on a trusted domain and a user-input subdomain prefix.
// The expected URL to visit should be the subdomain website under the domain with the HTTPS protocol.
// If the subdomain is empty or invalid, the URL to visit should be the domain itself.
//
// @param domain: The trusted domain.
// @param subdomain: The user-input subdomain to visit.
//
// @return: The URL to visit.
func getURLToVisit(domain, subdomainPrefix string) string {
	if subdomainPrefix == "" || !isValidSubdomain(subdomainPrefix) {
		return fmt.Sprintf("https://%s", domain)
	}
	return fmt.Sprintf("https://%s.%s", subdomainPrefix, domain)
}

// Helper function to check if the subdomain is valid.
func isValidSubdomain(subdomain string) bool {
	// Simplified validation: checks if subdomain contains only alphanumeric characters and hyphens,
	// and does not start or end with a hyphen.
	for _, r := range subdomain {
		if !(r >= 'a' && r <= 'z' || r >= 'A' && r <= 'Z' || r >= '0' && r <= '9' || r == '-') {
			return false
		}
	}
	return subdomain[0] != '-' && subdomain[len(subdomain)-1] != '-'
}
```