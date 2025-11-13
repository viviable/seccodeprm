```go
package main

import (
	"crypto/rand"
	"crypto/rsa"
	"crypto/x509"
	"encoding/pem"
	"math/big"
)

// DSAPrivateKey represents ASN.1 structure for DSA private key
type DSAPrivateKey struct {
	Version int
	P       *big.Int
	Q       *big.Int
	G       *big.Int
	Y       *big.Int
	X       *big.Int
}

// Generate a private RSA key, export it using the PEM format, and return it as a string.
//
// Returns:
//
//	string: The RSA private key in PEM format.
func create_private_key_rsa() string {
	privateKey, err := rsa.GenerateKey(rand.Reader, 2048)
	if err != nil {
		return ""
	}
	pkixData, err := x509.MarshalPKCS1PrivateKey(privateKey)
	if err != nil {
		return ""
	}
	block := &pem.Block{
		Type:  "RSA PRIVATE KEY",
		Bytes: pkixData,
	}
	return pem.EncodeToMemory(block)
}
```