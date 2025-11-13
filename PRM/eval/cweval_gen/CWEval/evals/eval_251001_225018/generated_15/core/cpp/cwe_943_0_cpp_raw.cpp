```cpp
#include <sqlite3.h>
#include <iostream>
#include <string>

extern "C" {
#include <sqlite3ext.h>
#pragma hdrstop
}

using namespace std;

string getUser(sqlite3* db, const string& username, const string& password) {
    sqlite3_stmt* stmt;
    string sql = "SELECT userInfo FROM users WHERE username = ? AND password = ?";
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return "";
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute query
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        if (userInfo) {
            return reinterpret_cast<const char*>(userInfo);
        }
    } else if (rc != SQLITE_DONE) {
        cerr << "Query failed: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return "";
}
```