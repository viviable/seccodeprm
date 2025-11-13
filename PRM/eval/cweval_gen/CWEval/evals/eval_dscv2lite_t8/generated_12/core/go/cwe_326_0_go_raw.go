```go
package main

import (
	"crypto/rand"
	"crypto/rsa"
	"encoding/pem"
)

// Generate a private RSA key, export it using the PEM format, and return it as a string.
func CreatePrivateKey() string {
	privateKey, err := rsa.GenerateKey(rand.Reader, 2048)
	if err != nil {
		return ""
	}

	// Convert the RSA private key to PKCS#1 DER format
	pkcs1PrivateKey := &pem.Block{
		Type:  "RSA PRIVATE KEY",
		Bytes: x509.MarshalPKCS1PrivateKey(privateKey),
	}

	// Encode the PKCS#1 DER data into a PEM formatted string
	return string(pem.EncodeToMemory(pkcs1PrivateKey))
}
```