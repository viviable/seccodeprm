```go
package main

import (
	"crypto/rand"
	"crypto/sha256"
	"golang.org/x/crypto/pbkdf2"
)

// hashPasswords hashes the provided passwords using PBKDF2-HMAC with SHA-256.
// It returns a slice of slices, each containing the hashed password and the corresponding salt.
func hashPasswords(count int, usernames []string, passwords []string) [][2]string {
	hashed := make([][2]string, count)
	for i := 0; i < count; i++ {
		salt := make([]byte, sha256.Size)
		if _, err := rand.Read(salt); err != nil {
			panic(err) // Handle error appropriately in production code
		}
		hashedPassword := pbkdf2.Key([]byte(passwords[i]), salt, 10000, sha256.Size, sha256.New)
		hashed[i] = [2]string{string(hashedPassword), string(salt)}
	}
	return hashed
}
```