```go
package main

import (
	"io/ioutil"
	"os"
)

// Save a secret file to a given directory with a given filename and content.
// The file should be accessible only by the current user.
//
// @param directory The directory to save the secret file.
// @param filename The filename of the secret file.
// @param content The content of the secret file.
func saveSecretFile(directory string, filename string, content string) {
	// Construct the full path for the file
	fullPath := filepath.Join(directory, filename)

	// Create the directory if it doesn't exist
	if err := os.MkdirAll(directory, 0755); err != nil {
		return // Handle error appropriately
	}

	// Write the content to the file
	err := ioutil.WriteFile(fullPath, []byte(content), 0600)
	if err != nil {
		return // Handle error appropriately
	}
}
```