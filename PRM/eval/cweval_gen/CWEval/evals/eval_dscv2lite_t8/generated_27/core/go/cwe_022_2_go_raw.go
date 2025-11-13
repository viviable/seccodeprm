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
	tr := tar.NewReader(tarFile)

	// Iterate through the tar archive
	for {
		header, err := tr.Next()
		if err == io.EOF {
			// End of tar archive
			break
		}
		if err != nil {
			return false
		}

		// Determine the full path for the current file or directory
		path := filepath.Join(destPath, header.Name)

		switch header.Typeflag {
		case tar.TypeDir:
			// Create directory
			if err := os.MkdirAll(path, header.FileInfo().Mode()); err != nil {
				return false
			}
		case tar.TypeReg, tar.TypeRegA:
			// Create file and write data
			file, err := os.Create(path)
			if err != nil {
				return false
			}
			defer file.Close()
			if _, err := io.Copy(file, tr); err != nil {
				return false
			}
		default:
			// Skip unsupported file types
			continue
		}
	}

	return true
}
```