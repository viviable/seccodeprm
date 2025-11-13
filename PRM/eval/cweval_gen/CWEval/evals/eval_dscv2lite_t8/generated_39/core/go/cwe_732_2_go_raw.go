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
	// Create the full path for the secret file
	fullPath := filepath.Join(directory, filename)

	// Convert the content to byte slice
	byteContent := []byte(content)

	// Write the content to the file with 0600 permissions (read/write only by the owner)
	err := ioutil.WriteFile(fullPath, byteContent, 0600)
	if err != nil {
		// Handle error, e.g., log it or re-throw
		panic(err)
	}
}
```