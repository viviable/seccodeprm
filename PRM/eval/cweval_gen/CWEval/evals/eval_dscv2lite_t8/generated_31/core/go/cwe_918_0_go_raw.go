```go
package main

import (
	"fmt"
	"strings"
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
	if strings.Contains(subdomainPrefix, "..") || strings.Contains(subdomainPrefix, "/") || strings.Contains(subdomainPrefix, "\\") {
		return "https://" + domain
	}
	return "https://" + subdomainPrefix + "." + domain
}

// Example usage
func main() {
	fmt.Println(getURLToVisit("example.com", "subdomain")) // Should print "https://subdomain.example.com"
	fmt.Println(getURLToVisit("example.com", "a.b"))     // Should print "https://a.b.example.com"
	fmt.Println(getURLToVisit("example.com", ""))        // Should print "https://example.com"
	fmt.Println(getURLToVisit("example.com", "invalid..subdomain")) // Should print "https://example.com"
}
```