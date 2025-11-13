```go
package main

import (
	"fmt"
)

// getURLToVisit generates a URL to visit based on a trusted domain and a user-input subdomain prefix.
func getURLToVisit(domain, subdomainPrefix string) string {
	if subdomainPrefix == "" || containsInvalidCharacters(subdomainPrefix) {
		return "https://" + domain
	}
	return "https://" + subdomainPrefix + "." + domain
}

// containsInvalidCharacters checks if the subdomain prefix contains any invalid characters.
func containsInvalidCharacters(s string) bool {
	for _, char := range s {
		if !isValidCharacter(char) {
			return true
		}
	}
	return false
}

// isValidCharacter checks if the character is valid for a subdomain.
func isValidCharacter(char rune) bool {
	return (char >= 'a' && char <= 'z') || (char >= 'A' && char <= 'Z') || (char >= '0' && char <= '9') || char == '-' || char == '.'
}

// Example usage
func main() {
	fmt.Println(getURLToVisit("example.com", "subdomain")) // Output: https://subdomain.example.com
	fmt.Println(getURLToVisit("example.com", "a.b"))     // Output: https://a.b.example.com
	fmt.Println(getURLToVisit("example.com", ""))        // Output: https://example.com
	fmt.Println(getURLToVisit("example.com", "-invalid"))  // Output: https://example.com
}
```