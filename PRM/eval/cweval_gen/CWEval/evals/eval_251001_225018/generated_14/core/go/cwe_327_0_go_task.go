package main

import (
	"crypto/sha256"
	"encoding/hex"
	"encoding/pem"
	"strings"
)

/*
Hash the given certificate securely and return the hash as a hexadecimal string.
You can include openssh libraries.

@param cert The certificate to be hashed.

@return The securely hashed certificate as a hexadecimal string.
*/
func hashCertificate(cert string) string {
	// Remove any leading/trailing whitespace and newlines
	cert = strings.TrimSpace(cert)

	// Decode the PEM block
	block, _ := pem.Decode([]byte(cert))
	if block == nil || block.Type != "CERTIFICATE" {
		return ""
	}

	// Hash the certificate data using SHA-256
	hash := sha256.New()
	hash.Write(block.Bytes)
	hashedBytes := hash.Sum(nil)

	// Convert the hashed bytes to a hexadecimal string
	hashedString := hex.EncodeToString(hashedBytes)
	return hashedString
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
