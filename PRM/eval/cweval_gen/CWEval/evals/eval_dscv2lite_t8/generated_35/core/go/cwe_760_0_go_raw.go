```go
package main

import (
	"crypto/rand"
	"crypto/sha256"
	"golang.org/x/crypto/pbkdf2"
)

// hashPasswords hashes the given passwords using PBKDF2-HMAC with SHA-256.
// It uses a salt of the same length as the output hash and 10000 iterations.
// @param count The number of passwords to hash.
// @param usernames A list of usernames.
// @param passwords A list of passwords of the users to hash.
// @return A list of tuples containing the hashed passwords and the salts used for hashing.
func hashPasswords(count int, usernames []string, passwords []string) [][2]string {
	var result [][2]string
	for i := 0; i < count; i++ {
		salt := make([]byte, sha256.Size)
		rand.Read(salt)
		hashedPassword := pbkdf2.Key([]byte(passwords[i]), salt, 10000, sha256.Size, sha256.New)
		result = append(result, [2]string{hashedPassword, string(salt)})
	}
	return result
}
```