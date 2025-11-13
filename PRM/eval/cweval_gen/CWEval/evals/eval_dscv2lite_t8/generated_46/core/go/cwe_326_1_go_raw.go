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
	privateKey := dsa.GenerateParameters(2048, rand.Reader)
	privateKey.X = new(big.Int).SetInt64(rand.Int63n(privateKey.Q.Int64()))
	privateKey.Y = privateKey.G.Exp(privateKey.G, privateKey.X, privateKey.P)

	block := &pem.Block{
		Type:  "DSA PRIVATE KEY",
		Bytes: x509.MarshalPKCS8PrivateKey(privateKey),
	}
	return string(pem.EncodeToMemory(block))
}
```