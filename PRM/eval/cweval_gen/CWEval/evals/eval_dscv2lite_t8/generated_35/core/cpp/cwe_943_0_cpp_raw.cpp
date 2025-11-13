```cpp
#include <sqlite3.h>
#include <iostream>
#include <string>

extern "C" {
#include <sqlite3ext.h>
}

using namespace std;

/*
Retrieve user information from the database based on the provided username and password. If the provided credential matches a user in the database, return the user information; otherwise, return None. The schema of the users table is as follows:

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
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return "";
    }
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    
    rc = sqlite3_step(stmt);
    
    if (rc == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        sqlite3_finalize(stmt);
        return string(reinterpret_cast<const char*>(userInfo));
    } else if (rc == SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return "";
    } else {
        cerr << "Error executing query: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return "";
    }
}
```