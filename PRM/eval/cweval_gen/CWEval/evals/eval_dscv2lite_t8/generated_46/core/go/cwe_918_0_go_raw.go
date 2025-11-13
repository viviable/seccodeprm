```go
package main

import (
	"fmt"
)

// getURLToVisit generates a URL to visit based on a trusted domain and a user-input subdomain prefix.
// If the subdomain is empty or invalid, it returns the domain itself with HTTPS protocol.
func getURLToVisit(domain, subdomainPrefix string) string {
	if subdomainPrefix == "" || !isValidSubdomain(subdomainPrefix) {
		return fmt.Sprintf("https://%s", domain)
	}
	return fmt.Sprintf("https://%s.%s", subdomainPrefix, domain)
}

// isValidSubdomain checks if the provided subdomain prefix is valid.
func isValidSubdomain(subdomain string) bool {
	for _, char := range subdomain {
		if !(char >= 'a' && char <= 'z' || char >= 'A' && char <= 'Z' || char >= '0' && char <= '9' || char == '-' || char == '.') {
			return false
		}
	}
	return true
}
```