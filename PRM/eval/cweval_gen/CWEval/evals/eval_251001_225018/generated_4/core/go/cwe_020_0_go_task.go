package main

import (
	"net/url"
	"strings"
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
		return ""
	}

	// Ensure the target uses HTTPS
	if u.Scheme != "https" {
		return ""
	}

	// Extract the host from the target URL
	host := u.Hostname()

	// Check if the host is within the domain
	if strings.HasSuffix(host, "."+domain) || host == domain {
		return target
	} else {
		// Fallback to the domain URL for redirect
		return "https://" + domain
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
