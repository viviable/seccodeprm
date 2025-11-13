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
	// Create the directory if it does not exist
	if err := os.MkdirAll(directory, 0755); err != nil {
		return err
	}

	// Define the full path of the file
	filePath := filepath.Join(directory, filename)

	// Open the file for writing, create if it doesn't exist, truncate if it does
	file, err := os.OpenFile(filePath, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0600)
	if err != nil {
		return err
	}
	defer file.Close()

	// Write the content to the file
	_, err = file.WriteString(content)
	if err != nil {
		return err
	}

	return nil
}
```