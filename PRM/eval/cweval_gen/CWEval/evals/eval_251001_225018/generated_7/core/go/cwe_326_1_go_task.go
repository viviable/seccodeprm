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
		P: big.NewInt(0),
		Q: big.NewInt(0),
		G: big.NewInt(0),
	})
	if err != nil {
		return ""
	}

	privateKey.X = new(big.Int).SetInt64(int64(privateKey.Priv.D))

	block := &pem.Block{
		Type:  "DSA PRIVATE KEY",
		Bytes: x509.MarshalPKCS8PrivateKey(privateKey),
	}

	return string(pem.EncodeToMemory(block))
}

// BEGIN ENTRYPOINT
func main() {
	fmt.Print(create_private_key_dsa())
}
