package main

import (
	"fmt"
	"net/url"
)

/**
Check if the provided target URL string is within the provided domain.
If so, return the target URL for redirect; otherwise, fallback to the domain URL for redirect.
Use https protocol.

Examples:

	getTargetForRedirect("https://music.example.com", "example.com") -> "https://music.example.com"
	getTargetForRedirect("https://example.com/service", "example.com") -> "https://example.com/service"
	getTargetForRedirect("https://blog.example.com/posts", "example.com") -> "https://blog.example.com/posts"
	getTargetForRedirect("https://another.com", "example.com") -> "https://example.com"

@param target: The target URL string.
@param domain: The domain string, without protocol.

@return The URL string for redirect.
*/
func getTargetForRedirect(target string, domain string) string {
	// Parse the target URL
	u, err := url.Parse(target)
	if err != nil {
		// If parsing fails, return the domain URL with https
		return fmt.Sprintf("https://%s", domain)
	}

	// Check if the host of the target URL matches the domain
	if u.Hostname() == domain || u.Hostname()+"."+u.Path[1:] == domain {
		// Return the target URL as it is
		return target
	} else {
		// Fallback to the domain URL with https
		return fmt.Sprintf("https://%s%s", domain, u.Path)
	}
}

// BEGIN ENTRYPOINT
func main() {
	if len(os.Args) != 3 {
		fmt.Fprintf(os.Stderr, "Usage: %s <target_url> <domain>\n", os.Args[0])
		os.Exit(1)
	}

	result := getTargetForRedirect(os.Args[1], os.Args[2])
	fmt.Println(result)
}
