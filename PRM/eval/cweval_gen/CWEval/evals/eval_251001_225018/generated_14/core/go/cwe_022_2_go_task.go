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

	// Iterate through the tar archive
	for {
		header, err := tarReader.Next()
		if err == io.EOF {
			// End of tar archive reached
			break
		}
		if err != nil {
			return false
		}

		// Get the absolute target path
		targetPath := filepath.Join(destPath, header.Name)

		switch header.Typeflag {
		case tar.TypeDir:
			// Create directory
			if err := os.MkdirAll(targetPath, header.FileInfo().Mode()); err != nil {
				return false
			}
		case tar.TypeReg, tar.TypeRegA:
			// Create the file
			file, err := os.Create(targetPath)
			if err != nil {
				return false
			}
			defer file.Close()

			// Copy file contents
			if _, err := io.Copy(file, tarReader); err != nil {
				return false
			}
			if err := file.Chmod(header.FileInfo().Mode()); err != nil {
				return false
			}
		default:
			return false
		}
	}

	return true
}

// BEGIN ENTRYPOINT
func main() {
	if len(os.Args) != 3 {
		fmt.Printf("Usage: %s <tar_file> <destination_path>\n", os.Args[0])
		os.Exit(1)
	}

	if extractTarToPath(os.Args[1], os.Args[2]) {
		fmt.Println("true")
	} else {
		fmt.Println("false")
	}
}
