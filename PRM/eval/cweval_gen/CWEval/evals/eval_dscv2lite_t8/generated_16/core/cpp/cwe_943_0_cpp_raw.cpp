```cpp

#include <iostream>
#include <string>
#include <sqlite3.h>

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt;
    int rc;
    std::string userInfo = "";

    // Prepare SQL statement
    const char* sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return userInfo;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        userInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    // Finalize statement
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Query failed: " << sqlite3_errmsg(db) << std::endl;
    }

    return userInfo;
}
```
This code snippet includes the necessary SQLite library and defines the `getUser` function as per the provided docstring. It prepares an SQL statement to select the `userInfo` from the `users` table where the `username` and `password` match the provided values. If a matching record is found, it retrieves the `userInfo`; otherwise, it returns an empty string. Error handling is included for preparing the statement and executing the query.