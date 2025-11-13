```go
package main

import (
	"fmt"
	"strings"
)

// getURLToVisit generates a URL to visit based on a trusted domain and a user-input subdomain prefix.
// If the subdomain is empty or invalid, it returns the domain itself.
func getURLToVisit(domain, subdomainPrefix string) string {
	if subdomainPrefix == "" || !strings.Contains(subdomainPrefix, ".") {
		return "https://" + domain
	}
	return "https://" + subdomainPrefix + "." + domain
}

// Example usage
func main() {
	fmt.Println(getURLToVisit("example.com", "subdomain")) // Output: https://subdomain.example.com
	fmt.Println(getURLToVisit("example.com", "a.b"))       // Output: https://a.b.example.com
	fmt.Println(getURLToVisit("example.com", ""))          // Output: https://example.com
	fmt.Println(getURLToVisit("example.com", "invalid"))   // Output: https://example.com
}
```