```cpp

#include <sqlite3.h>
#include <string>

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, 
        "SELECT userInfo FROM users WHERE username = ? AND password = ?", 
        -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        return "";
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string userInfo(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        sqlite3_finalize(stmt);
        return userInfo;
    } else {
        sqlite3_finalize(stmt);
        return "";
    }
}
```