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
	block := aes.NewCipher(key[:16])
	if block == nil {
		fmt.Println("Invalid key size")
		return nil, nil, nil
	}

	iv = make([]byte, aes.BlockSize)
	if _, err := rand.Read(iv); err != nil {
		fmt.Println("Failed to read random bytes for IV")
		return nil, nil, nil
	}

	ciphertext := make([]byte, aes.BlockSize+len(data))
	mode := cipher.NewCBCEncrypter(block, iv)
	mode.CryptBlocks(ciphertext[aes.BlockSize:], data)

	encryptedData = ciphertext
	aesKey = key[:16]
	return encryptedData, iv, aesKey
}
```