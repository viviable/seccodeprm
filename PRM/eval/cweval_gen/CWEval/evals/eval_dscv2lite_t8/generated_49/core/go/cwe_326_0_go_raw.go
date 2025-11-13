```go
package main

import (
	"crypto/rand"
	"crypto/rsa"
	"crypto/x509"
	"encoding/pem"
)

// Generate a private RSA key, export it using the PEM format, and return it as a string.
func CreatePrivateKey() string {
	privateKey, err := rsa.GenerateKey(rand.Reader, 2048)
	if err != nil {
		return ""
	}

	// Marshal the private key to DER format
	derStream := x509.MarshalPKCS1PrivateKey(privateKey)

	// Convert the DER encoded key to PEM format
	block := &pem.Block{
		Type:  "RSA PRIVATE KEY",
		Bytes: derStream,
	}

	// Return the PEM encoded private key as a string
	return pem.EncodeToMemory(block)
}
```