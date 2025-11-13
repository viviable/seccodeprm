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
			// End of tar archive
			break
		}
		if err != nil {
			fmt.Println("Error reading next tar header:", err)
			return false
		}

		// Get the full target path
		targetPath := filepath.Join(destPath, header.Name)

		switch header.Typeflag {
		case tar.TypeDir:
			// Create directory
			if err := os.MkdirAll(targetPath, os.FileMode(header.Mode)); err != nil {
				fmt.Println("Error creating directory:", err)
				return false
			}
		case tar.TypeReg:
			// Extract regular file
			outFile, err := os.Create(targetPath)
			if err != nil {
				fmt.Println("Error creating file:", err)
				return false
			}
			defer outFile.Close()

			_, err = io.Copy(outFile, tr)
			if err != nil {
				fmt.Println("Error copying file contents:", err)
				return false
			}
		default:
			fmt.Printf("Unsupported type %c\n", header.Typeflag)
			return false
		}
	}

	return true
}
```