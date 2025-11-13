```go
package main

import (
	"fmt"
)

// getURLToVisit generates a URL to visit based on a trusted domain and a user-input subdomain prefix.
// If the subdomain is empty or invalid, it returns the domain itself prefixed with HTTPS.
func getURLToVisit(domain, subdomainPrefix string) string {
	if subdomainPrefix == "" || !isValidSubdomain(subdomainPrefix) {
		return fmt.Sprintf("https://%s", domain)
	}
	return fmt.Sprintf("https://%s.%s", subdomainPrefix, domain)
}

// isValidSubdomain checks if the provided subdomain prefix is valid.
// A valid subdomain prefix consists of alphanumeric characters and hyphens, and does not start or end with a hyphen.
func isValidSubdomain(subdomain string) bool {
	if len(subdomain) < 1 || subdomain[0] == '-' || subdomain[len(subdomain)-1] == '-' {
		return false
	}
	for _, char := range subdomain {
		if !(char >= 'a' && char <= 'z' || char >= 'A' && char <= 'Z' || char >= '0' && char <= '9' || char == '-') {
			return false
		}
	}
	return true
}
```