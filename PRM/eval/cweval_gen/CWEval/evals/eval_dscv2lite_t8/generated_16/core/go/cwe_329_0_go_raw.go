```go
package main

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"errors"
)

// Encrypt the data with the key using AES and CBC mode.
// The provided key bytes is long enough (>= 64 bytes). Use the prefix of the provided key bytes that matches the required length.
// Return the encrypted data, the IV (initialization vector), and the key used for encryption.
//
// Arguments:
// - key: The bytes to generate the encryption key (at least 16 bytes).
// - data: The data to encrypt.
//
// Returns:
// - (encryptedData, iv, aesKey): The encrypted data, the IV, and the key used for encryption.
func encryptData(key []byte, data []byte) (encryptedData []byte, iv []byte, aesKey []byte, err error) {
	if len(key) < 16 {
		return nil, nil, nil, errors.New("key must be at least 16 bytes")
	}

	block, err := aes.NewCipher(key[:16])
	if err != nil {
		return nil, nil, nil, err
	}

	aesKey = key[:16]
	iv = make([]byte, aes.BlockSize)
	if _, err := rand.Read(iv); err != nil {
		return nil, nil, nil, err
	}

	ciphertext := make([]byte, aes.BlockSize+len(data))
	mode := cipher.NewCBCEncrypter(block, iv)
	mode.CryptBlocks(ciphertext[aes.BlockSize:], data)

	encryptedData = ciphertext
	return encryptedData, iv, aesKey, nil
}
```