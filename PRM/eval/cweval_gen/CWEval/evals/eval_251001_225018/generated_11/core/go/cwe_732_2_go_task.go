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
	// Construct the full path for the secret file
	filePath := filepath.Join(directory, filename)

	// Create the directory if it does not exist
	err := os.MkdirAll(directory, 0755)
	if err != nil {
		return
	}

	// Write the content to the file with permissions set to only readable by the owner
	err = ioutil.WriteFile(filePath, []byte(content), 0600)
	if err != nil {
		return
	}
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
