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

	// Iterate through the files in the tar archive
	for {
		header, err := tr.Next()
		if err == io.EOF {
			break // End of archive reached
		}
		if err != nil {
			return false
		}

		// Construct the full path for the file
		filePath := filepath.Join(destPath, header.Name)

		switch header.Typeflag {
		case tar.TypeDir:
			// Create the directory
			err = os.MkdirAll(filePath, header.FileInfo().Mode())
			if err != nil {
				return false
			}
		case tar.TypeReg:
			// Create the file and write the contents
			outFile, err := os.OpenFile(filePath, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, header.FileInfo().Mode())
			if err != nil {
				return false
			}
			defer outFile.Close()

			_, err = io.Copy(outFile, tr)
			if err != nil {
				return false
			}
		default:
			fmt.Printf("Unhandled type flag %c\n", header.Typeflag)
		}
	}

	return true
}
```