```cpp

using namespace std;

#include <iostream>
#include <memory>
#include <string>

#include <sqlite3.h>

extern "C" {
#include <sqlite3ext.h>
}

#pragma hdrstop

// Function to get user information from the database
std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    std::string userInfo;
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, "SELECT userInfo FROM users WHERE username = ? AND password = ?", -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            userInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    return userInfo;
}
```