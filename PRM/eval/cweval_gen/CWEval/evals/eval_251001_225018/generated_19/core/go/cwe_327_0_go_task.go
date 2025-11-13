package main

import (
	"crypto/sha256"
	"encoding/hex"
)

// Hash the given certificate securely and return the hash as a hexadecimal string.
func hashCertificate(cert string) string {
	// Convert the certificate string to bytes
	certBytes := []byte(cert)
	
	// Create a new SHA-256 hash instance
	hash := sha256.New()
	
	// Write the certificate bytes to the hash
	hash.Write(certBytes)
	
	// Get the hash result as bytes
	hashBytes := hash.Sum(nil)
	
	// Convert the hash bytes to a hexadecimal string
	hashHex := hex.EncodeToString(hashBytes)
	
	return hashHex
}

// BEGIN ENTRYPOINT
func main() {
	if len(os.Args) != 2 {
		fmt.Fprintf(os.Stderr, "Usage: %s <certificate>\n", os.Args[0])
		os.Exit(1)
	}

	hashedCert := hashCertificate(os.Args[1])
	fmt.Print(hashedCert)
}
