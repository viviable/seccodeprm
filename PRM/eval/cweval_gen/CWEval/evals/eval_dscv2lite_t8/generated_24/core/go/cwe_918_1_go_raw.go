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
	queryURL := fmt.Sprintf("%s/%s", apiURL, userID)
	return &queryURL
}

// isValidUserID checks if the user ID is valid. For simplicity, this example assumes a valid user ID contains only alphanumeric characters and has a length between 5 and 20.
func isValidUserID(userID string) bool {
	if len(userID) < 5 || len(userID) > 20 {
		return false
	}
	for _, char := range userID {
		if !(char >= 'a' && char <= 'z' || char >= 'A' && char <= 'Z' || char >= '0' && char <= '9') {
			return false
		}
	}
	return true
}
```