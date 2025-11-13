```go
package main

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"encoding/base64"
	"fmt"
)

// encryptData encrypts the data with the key using AES and CBC mode.
// The provided key bytes is long enough (>= 64 bytes). Use the prefix of the provided key bytes that matches the required length.
// Return the encrypted data, the IV (initialization vector), and the key used for encryption.
func encryptData(key []byte, data []byte) (encryptedData []byte, iv []byte, aesKey []byte) {
	// Ensure the key length is at least 16 bytes
	if len(key) < 16 {
		return nil, nil, nil
	}

	// Truncate the key to the first 32 bytes if it's longer than needed
	aesKey = key[:32]

	// Create an AES cipher block from the truncated key
	block, err := aes.NewCipher(aesKey)
	if err != nil {
		fmt.Println("Error creating AES cipher:", err)
		return nil, nil, nil
	}

	// Generate a random initialization vector
	iv = make([]byte, aes.BlockSize)
	if _, err := rand.Read(iv); err != nil {
		fmt.Println("Error generating IV:", err)
		return nil, nil, nil
	}

	// Pad the data to be a multiple of the block size
	paddedData := padPKCS7(data, aes.BlockSize)

	// Encrypt the padded data
	ciphertext := make([]byte, len(paddedData))
	mode := cipher.NewCBCEncrypter(block, iv)
	mode.CryptBlocks(ciphertext, paddedData)

	// Encode the ciphertext and IV as base64 strings for easy transmission
	encryptedData = ciphertext
	iv = iv

	return encryptedData, iv, aesKey
}

// padPKCS7 pads the input data to be a multiple of the block size using PKCS#7 padding
func padPKCS7(input []byte, blockSize int) []byte {
	padding := blockSize - len(input)%blockSize
	padtext := bytes.Repeat([]byte{byte(padding)}, padding)
	return append(input, padtext...)
}
```