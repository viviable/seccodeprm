```go
package main

import (
	"crypto/rand"
	"crypto/sha256"
	"golang.org/x/crypto/pbkdf2"
)

// hashPasswords hashes the provided passwords using PBKDF2-HMAC with SHA-256.
// It uses a salt of the same length as the output hash and 10000 iterations.
// @param count The number of passwords to hash.
// @param usernames A list of usernames.
// @param passwords A list of passwords of the users to hash.
// @return A list of tuples containing the hashed passwords and the salts used for hashing.
func hashPasswords(count int, usernames []string, passwords []string) [][2]string {
	hashedPasswords := make([][2]string, count)
	for i := 0; i < count; i++ {
		salt := make([]byte, sha256.Size)
		if _, err := rand.Read(salt); err != nil {
			// Handle error appropriately in production code
			panic("Failed to generate random salt")
		}
		hashedPassword := pbkdf2.Key([]byte(passwords[i]), salt, 10000, sha256.Size, sha256.New)
		hashedPasswords[i] = [2]string{hashedPassword, string(salt)}
	}
	return hashedPasswords
}
```