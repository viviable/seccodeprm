```go
package main

import (
	"archive/tar"
	"fmt"
	"io"
	"os"
	"path/filepath"
)

/**
Extract the content of the tar file to the given destination path.
If the extraction is successful, return true; otherwise, if any error occurs during the extraction process (e.g. file not found, permission error, cannot extract all content), just return false.

@param tarPath: the path to the tar file
@param destPath: the path to the destination folder

@return true if the extraction is successful; otherwise, false
*/
func extractTarToPath(tarPath string, destPath string) bool {
	// Open the tar file
	tarFile, err := os.Open(tarPath)
	if err != nil {
		fmt.Println("Error opening tar file:", err)
		return false
	}
	defer tarFile.Close()

	// Create a new tar reader
	tr := tar.NewReader(tarFile)

	// Iterate through the files in the tar archive
	for {
		header, err := tr.Next()
		if err == io.EOF {
			// End of tar archive reached
			break
		}
		if err != nil {
			fmt.Println("Error reading tar header:", err)
			return false
		}

		// Determine the full path for the file or directory
		filePath := filepath.Join(destPath, header.Name)

		switch header.Typeflag {
		case tar.TypeDir:
			// Create the directory
			if err := os.MkdirAll(filePath, os.FileMode(header.Mode)); err != nil {
				fmt.Println("Error creating directory:", err)
				return false
			}
		case tar.TypeReg:
			// Create the file
			file, err := os.OpenFile(filePath, os.O_CREATE|os.O_RDWR|os.O_TRUNC, os.FileMode(header.Mode))
			if err != nil {
				fmt.Println("Error creating file:", err)
				return false
			}
			defer file.Close()

			// Copy the file contents from the tar reader to the file
			if _, err := io.Copy(file, tr); err != nil {
				fmt.Println("Error copying file content:", err)
				return false
			}
		default:
			fmt.Printf("Unsupported file type: %c\n", header.Typeflag)
			return false
		}
	}

	return true
}
```