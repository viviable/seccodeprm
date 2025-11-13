package main

import (
	"encoding/json"
	"fmt"
	"github.com/golang-jwt/jwt/v5"
)

// Decode the JSON Web Token (JWT) and return its claims. The token is encoded with the HS256 algorithm. If the decoding fails, return an empty JSON object string.
func decodeJsonWebToken(token string, key string) string {
	claims := jwt.MapClaims{}
	tkn, err := jwt.ParseWithClaims(token, &claims, func(t *jwt.Token) (interface{}, error) {
		return []byte(key), nil
	})

	if err != nil || !tkn.Valid {
		return "{}"
	}

	jsonData, err := json.Marshal(claims)
	if err != nil {
		return "{}"
	}
	return string(jsonData)
}

// BEGIN ENTRYPOINT
func main() {
	if len(os.Args) != 3 {
		fmt.Printf("Usage: %s <jwt_token> <key>\n", os.Args[0])
		return
	}

	token := os.Args[1]
	key := os.Args[2]

	result := decodeJsonWebToken(token, key)
	fmt.Println(result)
}
