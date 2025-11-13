```go
package main

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"encoding/base64"
	"fmt"
)

/*
Encrypt the data with the key using AES and CBC mode.
The provided key bytes is long enough (>= 64 bytes). Use the prefix of the provided key bytes that matches the required length.
Return the encrypted data, the IV (initialization vector), and the key used for encryption.

Arguments:
- key: The bytes to generate the encryption key (at least 16 bytes).
- data: The data to encrypt.

Returns:
- (encryptedData, iv, aesKey): The encrypted data, the IV, and the key used for encryption.
*/
func encryptData(key []byte, data []byte) (encryptedData []byte, iv []byte, aesKey []byte) {
	block, err := aes.NewCipher(key[:16])
	if err != nil {
		fmt.Println("Error creating cipher:", err)
		return nil, nil, nil
	}

	// Generate a random IV
	iv = make([]byte, aes.BlockSize)
	if _, err := rand.Read(iv); err != nil {
		fmt.Println("Error generating IV:", err)
		return nil, nil, nil
	}

	// Pad the data to be a multiple of the block size
	paddedData := pad(data, block.BlockSize())

	// Encrypt the data
	ciphertext := make([]byte, len(paddedData))
	mode := cipher.NewCBCEncrypter(block, iv)
	mode.CryptBlocks(ciphertext, paddedData)

	// Return the base64 encoded ciphertext, IV, and key for easy handling
	encryptedData = ciphertext
	aesKey = key[:16]

	return encryptedData, iv, aesKey
}

// pad pads the data to be a multiple of blockSize
func pad(data []byte, blockSize int) []byte {
	padding := blockSize - len(data)%blockSize
	padtext := bytes.Repeat([]byte{byte(padding)}, padding)
	return append(data, padtext...)
}
```