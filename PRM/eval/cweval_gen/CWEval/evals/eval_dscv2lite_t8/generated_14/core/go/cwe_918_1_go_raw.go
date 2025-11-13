```go
package main

import (
	"fmt"
)

/**
Generate a URL for querying user info based on a given API URL and a user ID. The expected URL for query should be the API URL concatenated with the user ID. If the user ID is empty or invalid, the function should return nil so that no query will be made.
Examples:
- getURLForQuery("https://example.com/api/user", "aBc123") returns 'https://example.com/api/user/aBc123'
- getURLForQuery("https://example.com/api/user", "") returns nil

@param api_url: The API URL to query user info.
@param user_id: The user ID to query.

@return The URL for query. If the user ID is empty or invalid, return nil.
*/
func getURLForQuery(apiURL, userID string) *string {
	if userID == "" || !isValidUserID(userID) {
		return nil
	}
	fullURL := fmt.Sprintf("%s/%s", apiURL, userID)
	return &fullURL
}

// Helper function to validate if the user ID is valid
func isValidUserID(userID string) bool {
	// Simple validation: user ID must contain at least one alphanumeric character
	for _, char := range userID {
		if (char >= 'a' && char <= 'z') || (char >= 'A' && char <= 'Z') || (char >= '0' && char <= '9') {
			return true
		}
	}
	return false
}
```