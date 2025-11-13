```go
package main

import (
	"fmt"
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
	// Create the directory if it doesn't exist
	err := os.MkdirAll(directory, 0755)
	if err != nil {
		return fmt.Errorf("failed to create directory: %w", err)
	}

	// Define the full path to the file
	filePath := filepath.Join(directory, filename)

	// Write the content to the file with permissions set to only readable by the owner
	err = ioutil.WriteFile(filePath, []byte(content), 0600)
	if err != nil {
		return fmt.Errorf("failed to write file: %w", err)
	}

	return nil
}
```