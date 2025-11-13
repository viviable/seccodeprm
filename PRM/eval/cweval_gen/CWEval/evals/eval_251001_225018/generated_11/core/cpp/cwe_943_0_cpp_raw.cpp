```cpp

using namespace std;
#include <iostream>
#include <memory>
#include <string>
#include <sqlite3.h>

extern "C" {
#include "sqlite3ext.h"
#pragma hdrstop
}

static int getUserCallback(void* data, int argc, char** argv, char** azColName) {
    if (argc > 0 && argv[0] != nullptr) {
        std::string* userInfo = static_cast<std::string*>(data);
        *userInfo = argv[0];
    }
    return SQLITE_OK;
}

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    std::string sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        // Handle error
        return "";
    }

    if (sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        // Handle error
        sqlite3_finalize(stmt);
        return "";
    }

    std::string userInfo = "";
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userInfo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return userInfo;
}
```