```go
package main

import (
	"crypto/rand"
	"crypto/rsa"
	"encoding/pem"
	"fmt"
)

// Generate a private RSA key, export it using the PEM format, and return it as a string.
func CreatePrivateKey() string {
	privateKey, err := rsa.GenerateKey(rand.Reader, 2048)
	if err != nil {
		return fmt.Sprintf("Error generating private key: %v", err)
	}

	// Convert the RSA private key to PKCS1 DER encoded format
	pkcs1der := x509.MarshalPKCS1PrivateKey(privateKey)

	// Encode the PKCS1 DER data to PEM format
	privatePEM := pem.EncodeToMemory(&pem.Block{
		Type:  "RSA PRIVATE KEY",
		Bytes: pkcs1der,
	})

	return string(privatePEM)
}
```