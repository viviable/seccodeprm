```go
package main

import (
	"encoding/json"
	"github.com/golang-jwt/jwt/v5"
)

// Decode the JSON Web Token (JWT) and return its claims. The token is encoded with the HS256 algorithm. If the decoding fails, return an empty JSON object string.
// Use "github.com/golang-jwt/jwt/v5"
//
// @param token The JWT token to decode.
// @param key The key used in encoding.
//
// @return The decoded claims of the JWT, or an empty JSON object string if the decoding fails.
func decodeJsonWebToken(token string, key string) string {
	claims := jwt.MapClaims{}
	tokenObject, err := jwt.ParseWithClaims(token, &claims, func(t *jwt.Token) (interface{}, error) {
		return []byte(key), nil
	})
	if err != nil || !tokenObject.Valid {
		return "{}"
	}
	jsonData, err := json.Marshal(claims)
	if err != nil {
		return "{}"
	}
	return string(jsonData)
}
```