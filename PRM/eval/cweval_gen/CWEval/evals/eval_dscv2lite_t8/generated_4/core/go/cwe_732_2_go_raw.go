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
func saveSecretFile(directory string, filename string, content string) {
	// Create the full path for the file
	filePath := fmt.Sprintf("%s/%s", directory, filename)

	// Open the file for writing, creating it if it doesn't exist,
	// and truncating it if it does. Also, set the file permissions to 0600
	// which means the owner can read and write but no one else.
	file, err := os.OpenFile(filePath, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0600)
	if err != nil {
		fmt.Printf("Error opening file: %v\n", err)
		return
	}
	defer file.Close()

	// Write the content to the file
	_, err = file.WriteString(content)
	if err != nil {
		fmt.Printf("Error writing to file: %v\n", err)
		return
	}

	fmt.Println("Secret file saved successfully.")
}
```