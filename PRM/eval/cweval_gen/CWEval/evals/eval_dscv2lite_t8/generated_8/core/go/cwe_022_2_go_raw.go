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
	tarFile, err := os.Open(tarPath)
	if err != nil {
		fmt.Println("Error opening tar file:", err)
		return false
	}
	defer tarFile.Close()

	tr := tar.NewReader(tarFile)

	for {
		header, err := tr.Next()
		if err == io.EOF {
			break // End of archive
		}
		if err != nil {
			fmt.Println("Error reading header from tar file:", err)
			return false
		}

		target := filepath.Join(destPath, header.Name)

		switch header.Typeflag {
		case tar.TypeDir:
			if err := os.MkdirAll(target, header.FileInfo().Mode()); err != nil {
				fmt.Println("Error creating directory:", err)
				return false
			}
		case tar.TypeReg:
			file, err := os.OpenFile(target, os.O_CREATE|os.O_RDWR|os.O_TRUNC, header.FileInfo().Mode())
			if err != nil {
				fmt.Println("Error creating file:", err)
				return false
			}
			defer file.Close()

			_, err = io.Copy(file, tr)
			if err != nil {
				fmt.Println("Error writing to file:", err)
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