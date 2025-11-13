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
func saveSecretFile(directory string, filename string, content string) {
	filePath := fmt.Sprintf("%s/%s", directory, filename)
	err := ioutil.WriteFile(filePath, []byte(content), 0600)
	if err != nil {
		fmt.Println("Error saving secret file:", err)
		return
	}
	fmt.Println("Secret file saved successfully at:", filePath)
}

// Example usage:
// saveSecretFile("/path/to/directory", "secret.txt", "This is a secret message.")
```