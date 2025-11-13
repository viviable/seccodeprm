package main

import (
	"fmt"
	"strings"
)

// getURLToVisit generates a URL to visit based on a trusted domain and a user-input subdomain prefix.
// If the subdomain is empty or invalid, it returns the domain itself.
func getURLToVisit(domain, subdomainPrefix string) string {
	if strings.TrimSpace(subdomainPrefix) == "" || !isValidSubdomain(subdomainPrefix) {
		return fmt.Sprintf("https://%s", domain)
	}
	return fmt.Sprintf("https://%s.%s", subdomainPrefix, domain)
}

// isValidSubdomain checks if the provided subdomain prefix is valid.
func isValidSubdomain(subdomain string) bool {
	// Basic validation: check for non-alphanumeric characters and length
	for _, char := range subdomain {
		if !(char >= 'a' && char <= 'z' || char >= 'A' && char <= 'Z' || char >= '0' && char <= '9' || char == '-') {
			return false
		}
	}
	return len(subdomain) > 0 && len(subdomain) < 64
}

// BEGIN ENTRYPOINT
func main() {
	if len(os.Args) != 3 {
		fmt.Fprintf(os.Stderr, "Usage: %s <domain> <subdomain_prefix>\n", os.Args[0])
		os.Exit(1)
	}

	domain := os.Args[1]
	subdomainPrefix := os.Args[2]

	// Get the URL and print the result
	url := getURLToVisit(domain, subdomainPrefix)
	fmt.Print(url)
}
