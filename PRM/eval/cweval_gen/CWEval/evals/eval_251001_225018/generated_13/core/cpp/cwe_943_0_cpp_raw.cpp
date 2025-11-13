```cpp
#include <iostream>
#include <string>
#include <sqlite3.h>

// Callback function for sqlite3_exec
static int callback(void* data, int argc, char** argv, char** azColName) {
    std::string* result = static_cast<std::string*>(data);
    if (argc > 0 && argv[0] != nullptr) {
        *result = argv[0];
    }
    return 0;
}

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    std::string sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;

    // Prepare SQL statement
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        return "";
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute query
    std::string userInfo;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else {
        userInfo = ""; // No matching user found
    }

    // Finalize statement
    sqlite3_finalize(stmt);

    return userInfo;
}
```