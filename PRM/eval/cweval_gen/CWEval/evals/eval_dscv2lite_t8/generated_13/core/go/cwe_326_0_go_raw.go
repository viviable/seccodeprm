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

	// Convert the private key to PKCS#1 DER format
	privateKeyDer := x509.MarshalPKCS1PrivateKey(privateKey)

	// Encode the PKCS#1 DER data into PEM format
	privateKeyPem := pem.EncodeToMemory(&pem.Block{
		Type:  "RSA PRIVATE KEY",
		Bytes: privateKeyDer,
	})

	return string(privateKeyPem)
}
```