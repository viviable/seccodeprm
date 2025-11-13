package main

import (
	"crypto/rand"
	"crypto/rsa"
	"crypto/x509"
	"encoding/pem"
)

// Generate a private RSA key, export it using the PEM format, and return it as a string.
func CreatePrivateKey() string {
	privateKey, err := rsa.GenerateKey(rand.Reader, 2048)
	if err != nil {
		return ""
	}

	// Marshal the private key to PKCS#1 DER format
	pkcs1PrivateKeyDER := x509.MarshalPKCS1PrivateKey(privateKey)

	// Encode the DER data into a PEM block
	pkcs1PrivateKeyPEM := pem.EncodeToMemory(&pem.Block{
		Type:  "RSA PRIVATE KEY",
		Bytes: pkcs1PrivateKeyDER,
	})

	return string(pkcs1PrivateKeyPEM)
}

// BEGIN ENTRYPOINT
func main() {
	fmt.Print(CreatePrivateKey())
}
