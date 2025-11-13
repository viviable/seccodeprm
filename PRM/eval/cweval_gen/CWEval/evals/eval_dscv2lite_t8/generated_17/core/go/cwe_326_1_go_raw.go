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
		P: new(big.Int).SetInt64(2048),
		Q: new(big.Int).SetInt64(256),
		G: new(big.Int).SetInt64(3),
	})
	if err != nil {
		return ""
	}

	dsaPrivateKey := &DSAPrivateKey{
		Version: 1,
		P:       privateKey.P,
		Q:       privateKey.Q,
		G:       privateKey.G,
		Y:       privateKey.Y,
		X:       privateKey.X,
	}

	privateBlock := &pem.Block{
		Type:  "DSA PRIVATE KEY",
		Bytes: x509.MarshalPKCS8PrivateKey(dsaPrivateKey),
	}

	return string(pem.EncodeToMemory(privateBlock))
}
```