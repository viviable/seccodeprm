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

	// Create a new Tar reader from the file
	tr := tar.NewReader(tarFile)

	// Iterate through each header in the tar archive
	for {
		header, err := tr.Next()
		if err == io.EOF {
			// End of archive reached
			break
		}
		if err != nil {
			return false
		}

		// Define full target path
		targetPath := filepath.Join(destPath, header.Name)

		switch header.Typeflag {
		case tar.TypeDir:
			// If it's a directory, create it
			if err := os.MkdirAll(targetPath, 0755); err != nil {
				return false
			}
		case tar.TypeReg:
			// If it's a regular file, write its contents
			outFile, err := os.Create(targetPath)
			if err != nil {
				return false
			}
			defer outFile.Close()

			_, err = io.Copy(outFile, tr)
			if err != nil {
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