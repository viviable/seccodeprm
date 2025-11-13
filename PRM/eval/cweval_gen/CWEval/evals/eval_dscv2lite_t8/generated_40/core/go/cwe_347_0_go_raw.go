```go
package main

import (
	"encoding/json"
	"fmt"

	"github.com/golang-jwt/jwt/v5"
)

// decodeJsonWebToken decodes the JSON Web Token (JWT) and returns its claims. The token is encoded with the HS256 algorithm. If the decoding fails, return an empty JSON object string.
func decodeJsonWebToken(token string, key string) string {
	claims := jwt.MapClaims{}
	tkn, err := jwt.ParseWithClaims(token, &claims, func(token *jwt.Token) (interface{}, error) {
		return []byte(key), nil
	})

	if err != nil || !tkn.Valid {
		return "{}"
	}

	result, _ := json.Marshal(claims)
	return string(result)
}
```