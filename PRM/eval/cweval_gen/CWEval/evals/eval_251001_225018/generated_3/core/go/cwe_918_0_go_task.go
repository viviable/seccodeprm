package main

import (
	"fmt"
	"strings"
)

// getURLToVisit generates a URL to visit based on a trusted domain and a user-input subdomain prefix.
func getURLToVisit(domain, subdomainPrefix string) string {
	if strings.TrimSpace(subdomainPrefix) == "" || !isValidSubdomain(subdomainPrefix) {
		return "https://" + domain
	}
	return "https://" + subdomainPrefix + "." + domain
}

// isValidSubdomain checks if the provided subdomain prefix is valid.
func isValidSubdomain(subdomain string) bool {
	parts := strings.Split(subdomain, ".")
	for _, part := range parts {
		if len(part) < 1 || len(part) > 63 {
			return false
		}
		if !strings.AllUnicodeLetters(part) && !strings.Contains(part, "-") {
			return false
		}
		if part[0] == '-' || part[len(part)-1] == '-' {
			return false
		}
	}
	return true
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
