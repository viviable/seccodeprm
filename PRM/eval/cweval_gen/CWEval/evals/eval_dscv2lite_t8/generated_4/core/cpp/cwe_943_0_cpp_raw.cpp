```cpp
#include <sqlite3.h>
#include <string>

std::string getUser(sqlite3* db, const std::string& username, const std::string& password) {
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, 
        "SELECT userInfo FROM users WHERE username = ? AND password = ?", 
        -1, &stmt, nullptr);
    
    if (rc != SQLITE_OK) {
        // Handle error
        sqlite3_finalize(stmt);
        return "";
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* userInfo = sqlite3_column_text(stmt, 0);
        std::string result(reinterpret_cast<const char*>(userInfo));
        sqlite3_finalize(stmt);
        return result;
    } else {
        sqlite3_finalize(stmt);
        return "";
    }
}
```