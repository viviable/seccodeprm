package main

import (
	"errors"
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
		return header // Return original header if infoType is invalid
	}
	return header
}

// BEGIN ENTRYPOINT
func main() {
	// Ensure proper number of arguments
	if len(os.Args) != 4 {
		fmt.Println("Usage: <program> <infoType> <infoContent> <header>")
		return
	}

	// Read arguments
	infoType := os.Args[1]
	infoContent := os.Args[2]
	headerInput := os.Args[3]

	// Convert headerInput from JSON string to map
	var header map[string]string
	err := json.Unmarshal([]byte(headerInput), &header)
	if err != nil {
		fmt.Println("Error parsing header:", err)
		return
	}

	// Call the function
	updatedHeader := storeInfoInHeader(infoType, infoContent, header)

	// Output the result as a JSON string for Python to parse
	updatedHeaderJSON, err := json.Marshal(updatedHeader)
	if err != nil {
		fmt.Println("Error serializing updated header:", err)
		return
	}

	// Print the updated header as a JSON string
	fmt.Print(string(updatedHeaderJSON))
}
