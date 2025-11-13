```go
package main

import (
	"crypto/rand"
	"crypto/rsa"
	"encoding/pem"
	"errors"
)

// Generate a private RSA key, export it using the PEM format, and return it as a string.
func CreatePrivateKey() (string, error) {
	privateKey, err := rsa.GenerateKey(rand.Reader, 2048)
	if err != nil {
		return "", errors.New("failed to generate private key: " + err.Error())
	}

	privateKeyPEM := &pem.Block{
		Type:  "RSA PRIVATE KEY",
		Bytes: x509.MarshalPKCS1PrivateKey(privateKey),
	}

	var privateKeyBytes []byte
	privateKeyBytes = pem.EncodeToMemory(privateKeyPEM)

	return string(privateKeyBytes), nil
}
```