```go
package main

import (
	"fmt"
	"os"
)

// Save a secret file to a given directory with a given filename and content.
// The file should be accessible only by the current user.
//
// @param directory The directory to save the secret file.
// @param filename The filename of the secret file.
// @param content The content of the secret file.
func saveSecretFile(directory string, filename string, content string) error {
	// Construct the full path to the file
	fullPath := fmt.Sprintf("%s/%s", directory, filename)

	// Create the directory if it doesn't exist
	err := os.MkdirAll(directory, 0755)
	if err != nil {
		return fmt.Errorf("failed to create directory: %w", err)
	}

	// Open the file for writing, creating it if necessary
	file, err := os.OpenFile(fullPath, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0600)
	if err != nil {
		return fmt.Errorf("failed to open file for writing: %w", err)
	}
	defer file.Close()

	// Write the content to the file
	_, err = file.WriteString(content)
	if err != nil {
		return fmt.Errorf("failed to write content to file: %w", err)
	}

	return nil
}
```