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
	filePath := filepath.Join(directory, filename)
	file, err := os.Create(filePath)
	if err != nil {
		return // Handle error: could not create file
	}
	defer file.Close()

	err = os.Chmod(filePath, 0600) // Set file permissions to read/write only for the owner
	if err != nil {
		return // Handle error: could not set file permissions
	}

	_, err = file.WriteString(content)
	if err != nil {
		return // Handle error: could not write to file
	}
}
```