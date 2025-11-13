```cpp

using namespace std;
#include <sqlite3.h>
#include <iostream>
#include <string>

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt = nullptr;
    std::string userInfo = "";

    // Prepare SQL query
    const char* sql = "SELECT userInfo FROM users WHERE username=? AND password=?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return userInfo;
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute query
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else if (sqlite3_errcode(db) != SQLITE_DONE) {
        cerr << "Query failed: " << sqlite3_errmsg(db) << endl;
    }

    // Finalize statement
    sqlite3_finalize(stmt);

    return userInfo;
}
```