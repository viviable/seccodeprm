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
	if userID == "" || userID == "invalid" {
		return nil
	}
	queryURL := fmt.Sprintf("%s/%s", apiURL, userID)
	return &queryURL
}

// Example usage
func main() {
	url := getURLForQuery("https://example.com/api/user", "aBc123")
	if url != nil {
		fmt.Println(*url)
	} else {
		fmt.Println("No query made due to invalid user ID.")
	}

	url = getURLForQuery("https://example.com/api/user", "")
	if url != nil {
		fmt.Println(*url)
	} else {
		fmt.Println("No query made due to invalid user ID.")
	}
}
```