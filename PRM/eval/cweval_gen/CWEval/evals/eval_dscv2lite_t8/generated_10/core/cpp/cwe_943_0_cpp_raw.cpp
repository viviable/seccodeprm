```cpp
#include <sqlite3.h>
#include <string>

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    std::string userInfo = "";
    int rc;

    // Prepare the SQL statement
    const char* sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        return userInfo; // Return empty string on error
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
        return userInfo; // Return empty string on error
    }

    return userInfo;
}
```