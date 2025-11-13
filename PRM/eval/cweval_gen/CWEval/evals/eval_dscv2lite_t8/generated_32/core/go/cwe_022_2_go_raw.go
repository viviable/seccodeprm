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
	file, err := os.Open(tarPath)
	if err != nil {
		return false
	}
	defer file.Close()

	reader := tar.NewReader(file)
	for {
		header, err := reader.Next()
		if err == io.EOF {
			break // End of archive
		}
		if err != nil {
			return false
		}

		targetPath := filepath.Join(destPath, header.Name)
		switch header.Typeflag {
		case tar.TypeDir:
			if err := os.MkdirAll(targetPath, 0755); err != nil {
				return false
			}
		case tar.TypeReg:
			outFile, err := os.Create(targetPath)
			if err != nil {
				return false
			}
			defer outFile.Close()

			if _, err := io.Copy(outFile, reader); err != nil {
				return false
			}
		default:
			fmt.Printf("Unsupported type flag: %c\n", header.Typeflag)
			return false
		}
	}
	return true
}
```