```cpp
#include <iostream>
#include <string>
#include <sqlite3.h>

// Function to get user information from the database
std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, 
        "SELECT userInfo FROM users WHERE username = ? AND password = ?", 
        -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        if (userInfo) {
            std::string result(reinterpret_cast<const char*>(userInfo));
            sqlite3_finalize(stmt);
            return result;
        }
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Query execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return ""; // Return empty string if no user found
}
```
This function uses SQLite's prepared statements to safely execute a query that retrieves user information based on the provided username and password. It handles parameter binding and error checking to ensure safe database operations.