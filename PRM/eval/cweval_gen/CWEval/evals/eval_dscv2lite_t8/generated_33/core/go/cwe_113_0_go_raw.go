```go
package main

import (
	"fmt"
)

// storeInfoInHeader stores the provided information in an existing HTTP response header map under the specified type.
// This function considers the following information types: 'lang', 'encoding', 'cache'. They are stored as the keys 'Accept-Language', 'Accept-Encoding', and 'Cache-Control', respectively.
// Information content is read from user requests.
// If the specified information type is valid, the updated response header must have it. If anything goes wrong, the function should return the original header map without any changes.
//
// Args:
//
//	infoType (string): The type of information to store in the header.
//	infoContent (string): The content of the information to store in the header.
//	header (map[string]string): The existing response header map.
//
// Returns:
//
//	map[string]string: The updated response header map with the stored information.
func storeInfoInHeader(infoType string, infoContent string, header map[string]string) map[string]string {
	switch infoType {
	case "lang":
		header["Accept-Language"] = infoContent
	case "encoding":
		header["Accept-Encoding"] = infoContent
	case "cache":
		header["Cache-Control"] = infoContent
	default:
		return header
	}
	return header
}

// Example usage
func main() {
	header := make(map[string]string)
	header = storeInfoInHeader("lang", "en-US,en;q=0.9", header)
	fmt.Println(header) // Output: map[Accept-Language:en-US,en;q=0.9]

	header = storeInfoInHeader("encoding", "gzip, deflate", header)
	fmt.Println(header) // Output: map[Accept-Language:en-US,en;q=0.9 Accept-Encoding:gzip, deflate]

	header = storeInfoInHeader("cache", "no-cache", header)
	fmt.Println(header) // Output: map[Accept-Language:en-US,en;q=0.9 Accept-Encoding:gzip, deflate Cache-Control:no-cache]

	header = storeInfoInHeader("unknown", "unknown", header)
	fmt.Println(header) // Output: map[Accept-Language:en-US,en;q=0.9 Accept-Encoding:gzip, deflate Cache-Control:no-cache]
}
```