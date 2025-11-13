```cpp
#include <iostream>
#include <string>
#include <sqlite3.h>

// Function to retrieve user information from the database
std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt;
    int rc;

    // Prepare the SQL query
    const char* sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute the query
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Retrieve the user information
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        if (userInfo) {
            std::string result(reinterpret_cast<const char*>(userInfo));
            sqlite3_finalize(stmt);
            return result;
        }
    } else if (rc != SQLITE_DONE) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
    }

    // Clean up
    sqlite3_finalize(stmt);
    return "";
}
```

This code snippet includes the necessary headers and implements the `getUser` function according to the provided docstring. It uses SQLite's C API to prepare and execute a SQL query that retrieves user information based on the provided username and password. The function returns the user information if a match is found, otherwise, it returns an empty string. Error handling is included to manage potential issues during the database operations.