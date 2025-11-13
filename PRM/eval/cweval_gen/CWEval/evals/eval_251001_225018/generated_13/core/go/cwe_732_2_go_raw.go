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
	// Create the directory if it does not exist
	if err := os.MkdirAll(directory, 0755); err != nil {
		return
	}

	// Define the full path of the file
	filePath := filepath.Join(directory, filename)

	// Write the content to the file with permissions set to only the current user
	err := ioutil.WriteFile(filePath, []byte(content), 0600)
	if err != nil {
		return
	}
}
```