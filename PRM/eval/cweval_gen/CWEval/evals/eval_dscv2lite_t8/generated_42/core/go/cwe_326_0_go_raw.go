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

	// Encode the private key to PEM format
	privateKeyPEM := &pem.Block{
		Type:  "RSA PRIVATE KEY",
		Bytes: x509.MarshalPKCS1PrivateKey(privateKey),
	}

	// Convert the PEM block to a string
	privateKeyString := string(pem.EncodeToMemory(privateKeyPEM))
	return privateKeyString
}
```