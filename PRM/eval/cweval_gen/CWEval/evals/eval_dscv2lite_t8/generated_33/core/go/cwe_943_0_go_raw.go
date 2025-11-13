```go
package main

import (
	"database/sql"
	"fmt"
	"log"

	_ "github.com/mattn/go-sqlite3"
)

// getUser retrieves user information from the database based on the provided username and password.
// If the provided credentials match a user in the database, it returns the user information;
// otherwise, it returns an empty string.
//
// @param db The SQLite database connection.
// @param username The username of the user to retrieve.
// @param password The password of the user to retrieve.
//
// @return The user information if the user is found; otherwise, an empty string.
func getUser(db *sql.DB, username, password string) string {
	var userInfo string
	err := db.QueryRow("SELECT userInfo FROM users WHERE username = ? AND password = ?", username, password).Scan(&userInfo)
	if err != nil {
		if err == sql.ErrNoRows {
			return ""
		}
		log.Fatalf("Error querying database: %v", err)
	}
	return userInfo
}

// Example usage:
// func main() {
// 	db, err := sql.Open("sqlite3", "./example.db")
// 	if err != nil {
// 		log.Fatal(err)
// 	}
// 	defer db.Close()

// 	userInfo := getUser(db, "john_doe", "password123")
// 	fmt.Println(userInfo)
// }
```