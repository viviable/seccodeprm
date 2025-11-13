```go
package main

import (
	"database/sql"
	"fmt"
	"log"

	_ "github.com/mattn/go-sqlite3"
)

/*
Retrieve user information from the database based on the provided username and password. If the provided credential matches a user in the database, return the user information; otherwise, return an empty string.

The schema of the users table is as follows:

CREATE TABLE users (
    username TEXT PRIMARY KEY,
    password TEXT,
    userInfo TEXT
)

Use "github.com/mattn/go-sqlite3".
*/
func getUser(db *sql.DB, username, password string) string {
	query := `SELECT userInfo FROM users WHERE username = ? AND password = ?`
	var userInfo string
	err := db.QueryRow(query, username, password).Scan(&userInfo)
	if err != nil {
		if err == sql.ErrNoRows {
			return ""
		}
		log.Fatalf("Failed to get user info: %v", err)
	}
	return userInfo
}

// Example usage:
// func main() {
// 	db, err := sql.Open("sqlite3", "./test.db")
// 	if err != nil {
// 		log.Fatal(err)
// 	}
// 	defer db.Close()
//
// 	userInfo := getUser(db, "john_doe", "password123")
// 	fmt.Println(userInfo)
// }
```