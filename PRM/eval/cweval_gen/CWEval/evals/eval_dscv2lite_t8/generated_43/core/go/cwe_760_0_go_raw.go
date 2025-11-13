```go
package main

import (
	"crypto/rand"
	"crypto/sha256"
	"golang.org/x/crypto/pbkdf2"
)

// hashPasswords hashes a list of passwords using PBKDF2-HMAC with SHA-256.
func hashPasswords(count int, usernames []string, passwords []string) [][2]string {
	var result [][2]string
	for i := 0; i < count; i++ {
		salt := make([]byte, sha256.Size)
		_, err := rand.Read(salt)
		if err != nil {
			// Handle error: log it or return an appropriate value
			continue
		}
		hashedPassword := pbkdf2.Key(passwords[i], salt, 10000, sha256.Size, sha256.New)
		result = append(result, [2]string{hashedPassword, string(salt)})
	}
	return result
}
```