package main

import (
	"fmt"
)

// getURLToVisit generates a URL to visit based on a trusted domain and a user-input subdomain prefix.
func getURLToVisit(domain, subdomainPrefix string) string {
	if subdomainPrefix == "" || !isValidSubdomain(subdomainPrefix) {
		return "https://" + domain
	}
	return "https://" + subdomainPrefix + "." + domain
}

// isValidSubdomain checks if the provided subdomain prefix is valid.
func isValidSubdomain(subdomain string) bool {
	// Simple validation: check if the subdomain contains only letters, digits, and hyphens,
	// does not start or end with a hyphen, and has no consecutive hyphens.
	for i := 0; i < len(subdomain); i++ {
		if !(subdomain[i] >= 'a' && subdomain[i] <= 'z' ||
			subdomain[i] >= 'A' && subdomain[i] <= 'Z' ||
			subdomain[i] >= '0' && subdomain[i] <= '9' ||
			(i > 0 && i < len(subdomain)-1 && subdomain[i] == '-')) {
			return false
		}
	}
	return true
}

// Example usage
func main() {
	fmt.Println(getURLToVisit("example.com", "subdomain")) // Output: https://subdomain.example.com
	fmt.Println(getURLToVisit("example.com", "a.b"))     // Output: https://a.b.example.com
	fmt.Println(getURLToVisit("example.com", ""))        // Output: https://example.com
	fmt.Println(getURLToVisit("example.com", "-invalid")) // Output: https://example.com
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
