```cpp
#include <sqlite3.h>
#include <string>

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
std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, "SELECT userInfo FROM users WHERE username = ? AND password = ?", -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return "";
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        if (userInfo != nullptr) {
            std::string result(reinterpret_cast<const char*>(userInfo));
            sqlite3_finalize(stmt);
            return result;
        }
    }

    sqlite3_finalize(stmt);
    if (rc == SQLITE_DONE) {
        return ""; // No matching user found
    } else {
        return ""; // Other error occurred
    }
}
```