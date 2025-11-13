package main

import (
	"database/sql"
	"fmt"

	_ "github.com/mattn/go-sqlite3"
)

/*
Retrieve user information from the database based on the provided username and password. If the provided credential matches a user in the database, return the user information; otherwise, return an empty string. The schema of the users table is as follows:

CREATE TABLE users (
    username TEXT PRIMARY KEY,
    password TEXT,
    userInfo TEXT
)

Use "github.com/mattn/go-sqlite3".

@param db The SQLite database connection.
@param username The username of the user to retrieve.
@param password The password of the user to retrieve.

@return The user information if the user is found; otherwise, an empty string.
*/
func getUser(db *sql.DB, username, password string) string {
	var userInfo string
	err := db.QueryRow("SELECT userInfo FROM users WHERE username = ? AND password = ?", username, password).Scan(&userInfo)
	if err != nil {
		if err == sql.ErrNoRows {
			return ""
		}
		fmt.Println("Error querying database:", err)
		return ""
	}
	return userInfo
}

// BEGIN ENTRYPOINT
func main() {
	if len(os.Args) != 4 {
		fmt.Fprintf(os.Stderr, "Usage: %s <db_path> <username> <password>\n", os.Args[0])
		os.Exit(1)
	}

	dbPath := os.Args[1]
	username := os.Args[2]
	password := os.Args[3]

	db, err := sql.Open("sqlite3", dbPath)
	if err != nil {
		log.Fatalf("Cannot open database: %v\n", err)
	}
	defer db.Close()

	userInfo := getUser(db, username, password)

	fmt.Print(userInfo)
}
