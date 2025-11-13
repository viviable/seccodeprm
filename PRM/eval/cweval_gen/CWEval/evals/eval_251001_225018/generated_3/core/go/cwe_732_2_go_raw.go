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

	// Write the content to the file
	err := ioutil.WriteFile(fullPath, []byte(content), 0600)
	if err != nil {
		// Handle error: log it or return an error
		fmt.Println("Error writing secret file:", err)
		return
	}
}
```