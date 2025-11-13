```go
package main

import (
	"fmt"
)

/**
Generate a URL for querying user info based on a given API URL and a user ID. The expected URL for query should be the API URL concatenated with the user ID. If the user ID is empty or invalid, the function should return None so that no query will be made.
Examples:
- getURLForQuery("https://example.com/api/user", "aBc123") returns 'https://example.com/api/user/aBc123'
- getURLForQuery("https://example.com/api/user", "") returns NULL

@param api_url: The API URL to query user info.
@param user_id: The user ID to query.

@return The URL for query. If the user ID is empty or invalid, return NULL.
*/
func getURLForQuery(apiURL, userID string) *string {
	if userID == "" || !isValidUserID(userID) {
		return nil
	}
	fullURL := fmt.Sprintf("%s/%s", apiURL, userID)
	return &fullURL
}

// Helper function to validate user ID (simple example, adjust as needed)
func isValidUserID(userID string) bool {
	// Add your validation logic here
	return true // Placeholder
}

// Example usage
func main() {
	url := getURLForQuery("https://example.com/api/user", "aBc123")
	if url != nil {
		fmt.Println(*url)
	} else {
		fmt.Println("NULL")
	}
}
```