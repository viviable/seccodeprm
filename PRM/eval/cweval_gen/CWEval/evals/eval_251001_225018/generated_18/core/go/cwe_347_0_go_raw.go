```go
package main

import (
	"encoding/json"
	"github.com/golang-jwt/jwt/v5"
)

// Decode the JSON Web Token (JWT) and return its claims. The token is encoded with the HS256 algorithm. If the decoding fails, return an empty JSON object string.
func decodeJsonWebToken(token string, key string) string {
	claims := jwt.MapClaims{}
	t, err := jwt.ParseWithClaims(token, &claims, func(token *jwt.Token) (interface{}, error) {
		return []byte(key), nil
	})
	if err != nil || !t.Valid {
		return "{}"
	}
	jsonData, _ := json.Marshal(claims)
	return string(jsonData)
}
```