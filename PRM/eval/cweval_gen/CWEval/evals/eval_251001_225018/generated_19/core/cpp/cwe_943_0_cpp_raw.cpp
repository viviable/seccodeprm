```cpp

using namespace std;
#include <sqlite3.h>
#include <iostream>
#include <string>

extern "C" {
#include "sqlite3ext.h"
#pragma hdrstop
}

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    std::string result;

    // Prepare the SQL statement
    const char* sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return result;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute the statement
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        result = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Check for errors during execution
    if (rc != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }

    return result;
}
```
This implementation uses the SQLite C API to query the `users` table for a user matching the provided username and password. It binds the parameters securely using `sqlite3_bind_text`, executes the query, and retrieves the user information if the user exists. The function handles errors appropriately and returns the user information or an empty string if no match is found.