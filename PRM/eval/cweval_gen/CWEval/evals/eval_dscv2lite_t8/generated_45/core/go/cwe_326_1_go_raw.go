```go
package main

import (
	"crypto/rand"
	"crypto/dsa"
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

// Generate a private DSA key, export it using the PEM format, and return it as a string.
//
// Returns:
//
//	string: The DSA private key in PEM format.
func create_private_key_dsa() string {
	// Generate a new DSA private key
	privateKey, err := dsa.GenerateParameters(2048, rand.Reader)
	if err != nil {
		return ""
	}

	// Create a DSAPrivateKey struct from the generated parameters
	dsaPrivateKey := &DSAPrivateKey{
		Version: 0,
		P:       privateKey.P,
		Q:       privateKey.Q,
		G:       privateKey.G,
		Y:       privateKey.Y,
		X:       privateKey.X,
	}

	// Encode the private key to PEM format
	block := &pem.Block{
		Type:  "DSA PRIVATE KEY",
		Bytes: dsa.MarshalPrivateKey(privateKey),
	}

	// Return the PEM-encoded private key as a string
	return string(pem.EncodeToMemory(block))
}
```