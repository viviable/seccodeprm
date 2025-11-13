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
func saveSecretFile(directory string, filename string, content string) error {
	// Create the full path for the secret file
	fullPath := filepath.Join(directory, filename)

	// Define the file permissions to ensure it's readable and writable only by the current user
	fileMode := os.FileMode(0600)

	// Write the content to the file with the specified permissions
	err := ioutil.WriteFile(fullPath, []byte(content), fileMode)
	if err != nil {
		return err
	}

	return nil
}
```