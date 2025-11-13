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
		hdr, err := tr.Next()
		if err == io.EOF {
			// End of tar archive reached
			break
		}
		if err != nil {
			fmt.Println("Error reading tar header:", err)
			return false
		}

		// Determine the target path for the file or directory
		targetPath := filepath.Join(destPath, hdr.Name)

		switch hdr.Typeflag {
		case tar.TypeDir:
			// Create the directory
			err = os.MkdirAll(targetPath, 0755)
			if err != nil {
				fmt.Println("Error creating directory:", err)
				return false
			}
		case tar.TypeReg, tar.TypeRegA:
			// Extract the file
			file, err := os.OpenFile(targetPath, os.O_CREATE|os.O_RDWR|os.O_TRUNC, os.FileMode(hdr.Mode))
			if err != nil {
				fmt.Println("Error opening file for writing:", err)
				return false
			}
			defer file.Close()

			_, err = io.Copy(file, tr)
			if err != nil {
				fmt.Println("Error copying file content:", err)
				return false
			}
		default:
			fmt.Printf("Unsupported type flag: %c\n", hdr.Typeflag)
			return false
		}
	}

	return true
}
```