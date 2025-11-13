```cpp

using namespace std;
#include <sqlite3.h>
#include <iostream>
#include <string>

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    std::string userInfo;

    // Prepare the SQL statement
    int rc = sqlite3_prepare_v2(db, "SELECT userInfo FROM users WHERE username = ? AND password = ?", -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return userInfo;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute the query
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        userInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    // Finalize the statement
    sqlite3_finalize(stmt);

    // Check for errors
    if (rc != SQLITE_DONE) {
        cerr << "Query failed: " << sqlite3_errmsg(db) << endl;
    }

    return userInfo;
}
```
This function uses the SQLite C API to execute a prepared SQL query that retrieves the `userInfo` for a given `username` and `password`. It binds the provided username and password to the placeholders in the SQL statement, executes the query, and returns the retrieved user information. If no matching user is found or an error occurs, it returns an empty string.