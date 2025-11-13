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

	// Iterate through the tar archive entries
	for {
		header, err := tarReader.Next()
		if err == io.EOF {
			// End of tar archive reached
			break
		}
		if err != nil {
			return false
		}

		// Construct the full path for the entry
		entryPath := filepath.Join(destPath, header.Name)

		switch header.Typeflag {
		case tar.TypeDir:
			// Create directory
			if err := os.MkdirAll(entryPath, header.FileInfo().Mode()); err != nil {
				return false
			}
		case tar.TypeReg:
			// Extract regular file
			file, err := os.Create(entryPath)
			if err != nil {
				return false
			}
			defer file.Close()

			_, err = io.Copy(file, tarReader)
			if err != nil {
				return false
			}
			err = file.Sync()
			if err != nil {
				return false
			}
		default:
			// Unsupported type
			fmt.Printf("Unsupported file type: %c\n", header.Typeflag)
			continue
		}
	}

	return true
}
```