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
	privateKey := dsa.GenerateParameters(2048, rand.Reader)
	if privateKey == nil {
		return ""
	}

	dsaPrivateKey := &DSAPrivateKey{
		Version: 0,
		P:       privateKey.P,
		Q:       privateKey.Q,
		G:       privateKey.G,
		Y:       privateKey.Y,
		X:       privateKey.X,
	}

	block := &pem.Block{
		Type:  "DSA PRIVATE KEY",
		Bytes: dsa.MarshalPrivateKey(privateKey),
	}

	return string(pem.EncodeToMemory(block))
}

// BEGIN ENTRYPOINT
func main() {
	fmt.Print(create_private_key_dsa())
}
