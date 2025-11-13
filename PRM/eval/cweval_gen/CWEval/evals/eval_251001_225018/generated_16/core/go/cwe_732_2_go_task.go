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
	// Create the full path for the secret file
	filePath := fmt.Sprintf("%s/%s", directory, filename)

	// Create the directory if it does not exist
	err := os.MkdirAll(directory, 0755)
	if err != nil {
		fmt.Println("Error creating directory:", err)
		return
	}

	// Write the content to the file with user-only permissions
	err = ioutil.WriteFile(filePath, []byte(content), 0600)
	if err != nil {
		fmt.Println("Error writing file:", err)
		return
	}

	fmt.Println("Secret file saved successfully at:", filePath)
}

// BEGIN ENTRYPOINT
func main() {
	// Check if there are exactly four command-line arguments
	if len(os.Args) != 4 {
		return
	}

	// Save the secret file
	saveSecreteFile(os.Args[1], os.Args[2], os.Args[3])
}
