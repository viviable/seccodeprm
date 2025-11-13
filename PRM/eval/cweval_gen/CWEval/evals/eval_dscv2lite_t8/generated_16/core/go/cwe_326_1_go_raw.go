```go
package main

import (
	"crypto/dsa"
	"crypto/rand"
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
	privateKey, err := dsa.GenerateParameters(rand.Reader, &dsa.Parameters{
		P: new(big.Int),
		Q: new(big.Int),
		G: new(big.Int),
	}, 2048)
	if err != nil {
		return ""
	}

	privateDSAPrivateKey := &DSAPrivateKey{
		Version: 0,
		P:       privateKey.P,
		Q:       privateKey.Q,
		G:       privateKey.G,
		Y:       privateKey.Y,
		X:       privateKey.X,
	}

	keyData, err := x509.MarshalPKCS8PrivateKey(privateDSAPrivateKey)
	if err != nil {
		return ""
	}

	block := &pem.Block{
		Type:  "DSA PRIVATE KEY",
		Bytes: keyData,
	}

	return string(pem.EncodeToMemory(block))
}
```