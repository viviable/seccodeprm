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

	// Create the destination directory if it doesn't exist
	err = os.MkdirAll(destPath, 0755)
	if err != nil {
		fmt.Println("Error creating destination directory:", err)
		return false
	}

	// Create a new tar reader
	tarReader := tar.NewReader(tarFile)

	for {
		header, err := tarReader.Next()
		if err == io.EOF {
			// End of archive
			break
		}
		if err != nil {
			fmt.Println("Error reading tar header:", err)
			return false
		}

		// Get the absolute target path
		targetPath := filepath.Join(destPath, header.Name)

		switch header.Typeflag {
		case tar.TypeDir:
			// Create directory
			err = os.MkdirAll(targetPath, header.FileInfo().Mode())
			if err != nil {
				fmt.Println("Error creating directory:", err)
				return false
			}
		case tar.TypeReg, tar.TypeRegA:
			// Create file
			file, err := os.OpenFile(targetPath, os.O_CREATE|os.O_RDWR|os.O_TRUNC, header.FileInfo().Mode())
			if err != nil {
				fmt.Println("Error creating file:", err)
				return false
			}
			defer file.Close()

			_, err = io.Copy(file, tarReader)
			if err != nil {
				fmt.Println("Error copying file data:", err)
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