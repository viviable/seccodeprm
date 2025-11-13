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

	// Create a new Tar reader
	tr := tar.NewReader(tarFile)

	// Iterate through the tar entries
	for {
		header, err := tr.Next()
		if err == io.EOF {
			// End of archive
			break
		}
		if err != nil {
			return false
		}

		// Get the full target path
		targetPath := filepath.Join(destPath, header.Name)

		switch header.Typeflag {
		case tar.TypeDir:
			// Create directory
			if err := os.MkdirAll(targetPath, header.FileInfo().Mode()); err != nil {
				return false
			}
		case tar.TypeReg, tar.TypeRegA:
			// Create file and write contents
			file, err := os.Create(targetPath)
			if err != nil {
				return false
			}
			defer file.Close()

			if _, err := io.Copy(file, tr); err != nil {
				return false
			}
		default:
			// Skip unsupported types
			continue
		}
	}

	return true
}
```