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
		return false
	}
	defer tarFile.Close()

	// Create a new tar reader
	tarReader := tar.NewReader(tarFile)

	// Extract each file from the tar archive
	for {
		header, err := tarReader.Next()
		if err == io.EOF {
			break // End of tar archive reached
		}
		if err != nil {
			return false
		}

		// Determine the full path to write the file
		filePath := filepath.Join(destPath, header.Name)

		switch header.Typeflag {
		case tar.TypeDir:
			// Create directory
			if err := os.MkdirAll(filePath, os.FileMode(header.Mode)); err != nil {
				return false
			}
		case tar.TypeReg:
			// Create file
			outFile, err := os.Create(filePath)
			if err != nil {
				return false
			}
			defer outFile.Close()

			// Copy file contents
			if _, err := io.Copy(outFile, tarReader); err != nil {
				return false
			}
		default:
			// Unsupported file type
			return false
		}
	}

	return true
}
```